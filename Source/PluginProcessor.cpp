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
    mStepperDataModel.reset ( new StepperSequencerDataModel() );
    addListener(&lfo);
    addListener(&sequencer);
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
    
    ampAhdEnv.setSampleRate ( sampleRate );
    modAhdEnv.setSampleRate ( sampleRate );
    sequencer.setSampleRate ( sampleRate );
    sines    .setSampleRate ( sampleRate );
    lfo      .setSampleRate ( sampleRate );
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
 
        processModMatrix(modEnv, lfoAmp, modSeqCurrentValue);
        
        auto calculatedPitch    = currentStepPitch + poratmentoAccumulator + modMatrix.pitch * 36.0f;
        auto calculatedFM       = pow (currentStepFM + modMatrix.fm, 2);
        auto calculatedModMulti = currentStepModMulti + modMatrix.modMulti * 2.0f;
        auto calculatedVolume   = level * ampEnv * (modMatrix.volume + 1);
        auto calculatedPanMod   = modMatrix.pan / 2.0f;
        
        currentSwingValue = jlimit ( 1.0f, 1.9f, *swingValue + modMatrix.swing );
        
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
    
//    for (int stepIndex = 0; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
//    {
//        String parameterName {"step"};
//        parameterName << stepIndex + 1 << "Pitch";
//        DBG(*mParameters.getRawParameterValue(parameterName));
//    }
}

void FmseqnessAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr && xmlState->hasTagName (mParameters.state.getType()))
        mParameters.replaceState (ValueTree::fromXml (*xmlState));
    
    sendActionMessage ( "Preset Loaded" );
    
//    for (int stepIndex = 0; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
//    {
//        String parameterName {"step"};
//        parameterName << stepIndex + 1 << "Pitch";
//        DBG(*mParameters.getRawParameterValue(parameterName));
//    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FmseqnessAudioProcessor();
}


StepperSequencerDataModel& FmseqnessAudioProcessor::getStepperDataModel()
{
    return *mStepperDataModel;
}

AudioProcessorValueTreeState& FmseqnessAudioProcessor::getParametersTree()
{
    return mParameters;
}

void FmseqnessAudioProcessor::trigger()
{
    const int stepIndex = sequencer.getCurrentStepIndex();
    currentStep->store(stepIndex);
//    currentStepPitch = (int)mStepperDataModel->pitchValues.values[stepIndex]->getValue() + basePitch->load();
    currentStepPitch = *mParameters.getRawParameterValue(STEPS_PITCH[stepIndex]) + *basePitch;
    targetPitch = getNextStepPitch();
    isNextStepGlide = getNextStepGlide();
    poratmentoAccumulator = 0.0f;
    
//    auto gateState = mStepperDataModel->gateStateValues.values[stepIndex];
    auto gateState = (int)*mParameters.getRawParameterValue(STEPS_GATE[stepIndex]);
    
    int lfo2Restart = lfoRestart->load();
    
    if (lfo2Restart == 1)
        lfo.restart();
    
    if (gateState == GATE_OFF)
        return;
    
    if (gateState == GATE_ON)
    {
        auto swingFactor = stepIndex % 2 == 0 ? swingValue->load() : 2.0f - swingValue->load();
        float stepLength = mStepperDataModel->getStepLength (stepIndex);
        
        if (int(stepLength) % 2 == 1)
            stepLength = stepLength -  1 + swingFactor;
        
        ampAhdEnv.reset (ampEnv, stepLength);
        ampAhdEnv.state = PlayState::play;
        modAhdEnv.reset (ampEnv, stepLength);
        modAhdEnv.state = PlayState::play;
        currentStepFM = mStepperDataModel->fmValues.values[stepIndex];
        currentStepModMulti = mStepperDataModel->modMultiValues.values[stepIndex];
        currentStepModMulti = getModulatorMultiFrom01(currentStepModMulti);
        modSeqCurrentValue  = mStepperDataModel->seqModValues.values[stepIndex];
    }
    
    portamentoPitchUnit = (targetPitch - currentStepPitch ) / (portamento->load() * getNumberOfSamplesInStep());
    portamentoPitchUnit = isNextStepGlide ? portamentoPitchUnit : 0.0f;
    portamentoCountDown = getNumberOfSamplesInStep() * (1.0 - portamento->load());
    
    if (lfo2Restart == 2 && stepIndex == firstStepIndex->load())
        lfo.restart();
}

AHDEnvDataModel& FmseqnessAudioProcessor::getAmpAHDEnvDataModel()
{
    return mAmpAHDEnvModel;
}

AHDEnvDataModel& FmseqnessAudioProcessor::getModAHDEnvDataModel()
{
    return mModAHDEnvModel;
}

int FmseqnessAudioProcessor::getNumberOfSamplesInStep()
{
    int    step = currentStep->load();
    float  swingFactor = step % 2 == 0 ? swingValue->load() : 2.0f - swingValue->load();
    return swingFactor * currentSampleRate * (60 / tempo->load()) / 4;
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
        return mStepperDataModel->gateStateValues.values[int(firstStepIndex->load())];
    
    else if (currentStep->load() == MAX_NUM_OF_STEPS - 1)
        return mStepperDataModel->gateStateValues.values[0] + basePitch->load();
    
    return mStepperDataModel->gateStateValues.values[int(currentStep->load()) + 1];
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

float FmseqnessAudioProcessor::getModulatorMultiFrom01 (float value)
{
    float returnValue = 0.0f;
    
    if (value == 0.0f)
        returnValue = 0.25;
    else if (value == 1.0f/9.0f )
        returnValue = 0.5;
    else if (value == 2.0f/9.0f )
        returnValue = 1;
    else if (value == 3.0f/9.0f )
        returnValue = 2;
    else if (value == 4.0f/9.0f )
        returnValue = 3;
    else if (value == 5.0f/9.0f )
        returnValue = 4;
    else if (value == 6.0f/9.0f )
        returnValue = 5;
    else if (value == 7.0f/9.0f )
        returnValue = 6;
    else if (value == 8.0f/9.0f )
        returnValue = 7;
    else if (value == 9.0f/9.0f )
        returnValue = 8;
    
    return returnValue;
}


void FmseqnessAudioProcessor::processModMatrix(float env, float lfo, float modSeq)
{
    float sources[3] {env, lfo, modSeq};
    float results[7] {0.0f};
    
    for (int mod = 0; mod <= 3; mod++)
        results[ int ( *matrix.mods[mod].destination ) ] += sources[ int ( *matrix.mods[mod].source ) ] * *matrix.mods[mod].amount;

    modMatrix.fm         = isnan(results[0]) ? 0.0f : results[0];
    modMatrix.pitch      = isnan(results[1]) ? 0.0f : results[1];
    modMatrix.pan        = isnan(results[2]) ? 0.0f : results[2];
    modMatrix.volume     = isnan(results[3]) ? 0.0f : results[3];
    modMatrix.modMulti   = isnan(results[4]) ? 0.0f : results[4];
    modMatrix.swing      = isnan(results[6]) ? 0.0f : results[6];
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
    for (int mod = 1; mod <= 4; mod++)
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
