/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
FmseqnessAudioProcessor::FmseqnessAudioProcessor()
: AudioProcessor (BusesProperties().withOutput ("Output", AudioChannelSet::stereo(), true)),
mParameters (*this, nullptr, Identifier ("FMSeqness"), Parameters::createParameterLayout())
{
    addListener ( &lfo ) ;
    addListener ( &sequencer );
    refreshEnvelopesModels();
    matrixModelSetup();
}

FmseqnessAudioProcessor::~FmseqnessAudioProcessor()
{
}

//==============================================================================
const String FmseqnessAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FmseqnessAudioProcessor::acceptsMidi() const
{
    return true;
}

bool FmseqnessAudioProcessor::producesMidi() const
{
    return false;
}

bool FmseqnessAudioProcessor::isMidiEffect() const
{
    return false;
}

double FmseqnessAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FmseqnessAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FmseqnessAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FmseqnessAudioProcessor::setCurrentProgram (int index)
{
}

const String FmseqnessAudioProcessor::getProgramName (int index)
{
    return {};
}

void FmseqnessAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FmseqnessAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    currentSampleRate = sampleRate;
    
    ampAhdEnv .setSampleRate ( sampleRate );
    modAhdEnv .setSampleRate ( sampleRate );
    sequencer .setSampleRate ( sampleRate );
    sines     .setSampleRate ( sampleRate );
    lfo       .setSampleRate ( sampleRate );
}

void FmseqnessAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FmseqnessAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FmseqnessAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    auto level = 0.5f;
    auto* leftBuffer  = buffer.getWritePointer (0, buffer.getSample(0, 0));
    auto* rightBuffer = buffer.getWritePointer (1, buffer.getSample(1, 0));
    
    //Stops sound when sequencer is stopped
    if (isPlayingFloat->load() == 0.0f)
    {
        ampAhdEnv.startDecay();
        modAhdEnv.startDecay();
        gateAmp = 0.0f;
    }
    
    LFOShape shape = LFOShape (int(lfoShape->load()));
    
    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        if (sequencer.processToGetTrigger ( currentSwingValue ))
            trigger();
        
        lfoAmp = lfo.generate (shape);
        
        portamentoCountDown = portamentoCountDown > 0 ? portamentoCountDown - 1 : 0;

        if (isNextStepGlide && portamentoCountDown == 0)
            poratmentoAccumulator += portamentoPitchUnit;
        
        
        ampEnv = ampAhdEnv.process();
        modEnv = modAhdEnv.process();
        bool isEnvAmpControl = *ampControl == 0.0f;
        float amp = isEnvAmpControl ? ampEnv : gateAmp;
 
        processModMatrix (modEnv, lfoAmp, modSeqCurrentValue);
        
        auto calculatedPitch    = currentStepPitch + poratmentoAccumulator + modMatrix.pitch * 36.0f;
        auto calculatedFM       = pow (jmax (currentStepFM + modMatrix.fm, 0.0f), 2);
        auto calculatedModMulti = currentStepModMulti + modMatrix.modMulti * 2.0f;
        auto calculatedVolume   = level * amp * (modMatrix.volume + 1);
        auto calculatedPanMod   = modMatrix.pan / 2.0f;
                
        auto currentSample = sines.generate(calculatedPitch, calculatedFM, calculatedModMulti) * calculatedVolume;
        
        leftBuffer[sample]  = currentSample * (0.5 + calculatedPanMod);
        rightBuffer[sample] = currentSample * (0.5 - calculatedPanMod);
    }
}

//==============================================================================
bool FmseqnessAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FmseqnessAudioProcessor::createEditor()
{
    return new FmseqnessAudioProcessorEditor (*this);
}

//==============================================================================
void FmseqnessAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = mParameters.copyState();
    std::unique_ptr<XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void FmseqnessAudioProcessor::handleLoadedPreset()
{
    sequencer.updateNumberOfSteps();
    currentSwingValue = *swingValue;
    sendActionMessage ( "Preset Loaded" );
}

void FmseqnessAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr && xmlState->hasTagName (mParameters.state.getType()))
        mParameters.replaceState (ValueTree::fromXml (*xmlState));
    
    handleLoadedPreset();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FmseqnessAudioProcessor();
}


AudioProcessorValueTreeState& FmseqnessAudioProcessor::getParametersTree()
{
    return mParameters;
}

void FmseqnessAudioProcessor::trigger()
{
    const int stepIndex = sequencer.getCurrentStepIndex();
    currentStep->store(stepIndex);
    currentStepPitch = *mParameters.getRawParameterValue(STEPS_PITCH[stepIndex]) + *basePitch;
    targetPitch = getNextStepPitch();
    isNextStepGlide = getNextStepGlide();
    poratmentoAccumulator = 0.0f;
    
    
    int gateState = *mParameters.getRawParameterValue(STEPS_GATE[stepIndex]);
    
    int restartLFO = lfoRestart->load();
    
    if (LFO_RESTART_OPTIONS[restartLFO] == "Step")
        lfo.restart();
    
    else if (LFO_RESTART_OPTIONS[restartLFO] == "Pattern" && stepIndex == firstStepIndex->load())
        lfo.restart();
    
    if (gateState == GATE_OFF)
    {
        gateAmp = 0.0f;
        return;
    }
    
    
    
    if (gateState == GATE_ON)
    {
        //Here is the only place where swing modulation may take place, to avoid clicks.
        if ( sequencer.isCurrentStepSwinged() )
            currentSwingValue = jlimit ( SWING_MIN_VALUE, SWING_MAX_VALUE, *swingValue + modMatrix.swing );

        gateAmp = 1.0f;
        float stepLength = getTotalStepLength(stepIndex);
        ampAhdEnv.reset (stepLength);
        modAhdEnv.reset (stepLength);
        ampAhdEnv.start();
        modAhdEnv.start();
        sines.restart();
        
        currentStepFM       = *mParameters.getRawParameterValue ( STEPS_FM[stepIndex] );
        currentStepModMulti = *mParameters.getRawParameterValue ( STEPS_MOD_MULTI[stepIndex] );
        currentStepModMulti = getModulatorMultiFrom01           ( currentStepModMulti );
        modSeqCurrentValue  = *mParameters.getRawParameterValue ( STEPS_MOD_SEQ[stepIndex] );
    }
    
    portamentoPitchUnit = (targetPitch - currentStepPitch ) / (portamento->load() * getNumberOfSamplesInStep(true));
    portamentoPitchUnit = isNextStepGlide ? portamentoPitchUnit : 0.0f;
    portamentoCountDown = getNumberOfSamplesInStep(true) * (1.0 - portamento->load());
    
}

AHDEnvDataModel& FmseqnessAudioProcessor::getAmpAHDEnvDataModel()
{
    return mAmpAHDEnvModel;
}

AHDEnvDataModel& FmseqnessAudioProcessor::getModAHDEnvDataModel()
{
    return mModAHDEnvModel;
}

int FmseqnessAudioProcessor::getNumberOfSamplesInStep(bool considerSwing)
{
    auto swingFactor = 1.0f;
    
    if (considerSwing)
        swingFactor = sequencer.isCurrentStepSwinged() ? 2.0f - currentSwingValue : currentSwingValue;
    
    return swingFactor * currentSampleRate * (60 / tempo->load()) / SIXTEEN_DIVEDER;
}

float FmseqnessAudioProcessor::getNextStepPitch()
{
    int step = *currentStep;
    
    if (step == lastStepIndex->load())
        return *mParameters.getRawParameterValue(STEPS_PITCH[firstStepIndex->load()]) + basePitch->load();
    
    else if (step == MAX_NUM_OF_STEPS - 1)
        return *mParameters.getRawParameterValue(STEPS_PITCH[0]) + basePitch->load();
    
    return *mParameters.getRawParameterValue(STEPS_PITCH[step + 1]) + basePitch->load();
}

bool FmseqnessAudioProcessor::getNextStepGlide()
{
    if (currentStep->load() == lastStepIndex->load())
        return mParameters.getRawParameterValue(STEPS_GATE[int(*firstStepIndex)]);
    
    else if (*currentStep == MAX_NUM_OF_STEPS - 1)
        return *mParameters.getRawParameterValue(STEPS_GATE[0]) + *basePitch;
    
    return *mParameters.getRawParameterValue(STEPS_GATE[int(*currentStep) + 1]);
}

float FmseqnessAudioProcessor::getLfoAmp()
{
    return lfoAmp;
}

void FmseqnessAudioProcessor::updateSequncerNumberOfSteps()
{
    sequencer.updateNumberOfSteps();
}

void FmseqnessAudioProcessor::updateLFOAngle()
{
    lfo.updateAngleDelta();
}

float FmseqnessAudioProcessor::getModulatorMultiFrom01 (double value)
{
    int valueIndex = value * (NUM_OF_MOD_VALUES - 1) + 0.1;
    
    return MODULATOR_MULTI_VALUES[valueIndex];
}


void FmseqnessAudioProcessor::processModMatrix(float env, float lfo, float modSeq)
{
    float sources[MOD_MATRIX_SOURCES]      {env, lfo, modSeq};
    float results[MOD_MATRIX_DESTINATIONS] {0.0f};
    
    for (int mod = 0; mod < MOD_MATRIX_CHANNELS ; mod++)
        results[ int ( *matrix.mods[mod].destination ) ] += sources[ int ( *matrix.mods[mod].source ) ] * *matrix.mods[mod].amount;

    modMatrix.fm         = isnan(results[0]) ? 0.0f : results[0];
    modMatrix.pitch      = isnan(results[1]) ? 0.0f : results[1];
    modMatrix.pan        = isnan(results[2]) ? 0.0f : results[2];
    modMatrix.volume     = isnan(results[3]) ? 0.0f : results[3];
    modMatrix.modMulti   = isnan(results[4]) ? 0.0f : results[4];
    modMatrix.swing      = isnan(results[5]) ? 0.0f : results[5];
}

void FmseqnessAudioProcessor::refreshEnvelopesModels()
{
    mAmpAHDEnvModel.attack      = mParameters.getParameterAsValue ( "ampEnvAttack" );
    mAmpAHDEnvModel.attackCurve = mParameters.getParameterAsValue ( "ampEnvAttackCurve" );
    mAmpAHDEnvModel.hold        = mParameters.getParameterAsValue ( "ampEnvHold" );
    mAmpAHDEnvModel.decay       = mParameters.getParameterAsValue ( "ampEnvDecay" );
    mAmpAHDEnvModel.decayCurve  = mParameters.getParameterAsValue ( "ampEnvDecayCurve" );
    mAmpAHDEnvModel.level       = mParameters.getParameterAsValue ( "ampEnvLevel" );
    
    mModAHDEnvModel.attack      = mParameters.getParameterAsValue ( "modEnvAttack" );
    mModAHDEnvModel.attackCurve = mParameters.getParameterAsValue ( "modEnvAttackCurve" );
    mModAHDEnvModel.hold        = mParameters.getParameterAsValue ( "modEnvHold" );
    mModAHDEnvModel.decay       = mParameters.getParameterAsValue ( "modEnvDecay" );
    mModAHDEnvModel.decayCurve  = mParameters.getParameterAsValue ( "modEnvDecayCurve" );
    mModAHDEnvModel.level       = mParameters.getParameterAsValue ( "modEnvLevel" );
}

void FmseqnessAudioProcessor::matrixModelSetup()
{
    for (int mod = 1; mod <= MOD_MATRIX_CHANNELS; mod++)
    {
        String code { "mod" };
        code << mod << "Source";
        std::atomic<float>* source { mParameters.getRawParameterValue (code) };
        code = "mod";
        code << mod << "Amount";
        std::atomic<float>* amount { mParameters.getRawParameterValue (code) };
        code = "mod";
        code << mod << "Destination";
        std::atomic<float>* destination { mParameters.getRawParameterValue (code) };
        
        ModulationInstancModel module {amount, source, destination};
        
        matrix.mods.push_back(module);
    }
}

float FmseqnessAudioProcessor::getTotalStepLength (int stepIndex)
{
    int endIndex = stepIndex + 1;
    
    while (endIndex < MAX_NUM_OF_STEPS && *mParameters.getRawParameterValue(STEPS_GATE[endIndex]) == GATE_GLIDE)
        endIndex++;
    
    float length = endIndex - stepIndex;    
    
    if (int(length) % 2 == 1)
    {        
        if (sequencer.isCurrentStepSwinged())
            length = length - (currentSwingValue - 1.0f);
        else
            length = length + (currentSwingValue - 1.0f);
    }
        
    return length;
}
