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
mParameters (*this, nullptr, Identifier ("FMSeqness"),
{
    std::make_unique<AudioParameterFloat>  ("globalFMAmount", "Global FM Amount",
                                           NormalisableRange<float>(FM_AMNT_MIN_VALUE, FM_AMNT_MAX_VALUE, 0.001, FM_AMNT_SKEW_VALUE),
                                           FM_AMNT_SET_VALUE),
    std::make_unique<AudioParameterFloat>  ("modulatorMultiplier", "Modulator Multiplier",
                                           NormalisableRange<float>(MOD_MULTI_MIN_VALUE, MOD_MULTI_MAX_VALUE, 0.001, MOD_MULTI_SKEW_VALUE),
                                           MOD_MULTI_SET_VALUE),
    std::make_unique<AudioParameterFloat>  ("tempo", "Tempo",
                                           NormalisableRange<float>(TEMPO_MIN_VALUE, TEMPO_MAX_VALUE, 1.0, TEMPO_SKEW_VALUE),
                                           TEMPO_SET_VALUE),
    std::make_unique<AudioParameterInt>    ("steps", "Steps", MIN_NUM_OF_STEPS, MAX_NUM_OF_STEPS, DEF_NUM_OF_STEPS),
    std::make_unique<AudioParameterBool>   ("play", "Play", false),
    std::make_unique<AudioParameterFloat>  ("currentStep", "Current Step", NormalisableRange<float>(0.0, MAX_NUM_OF_STEPS, 1.0, 1.0), MAX_NUM_OF_STEPS),
    std::make_unique<AudioParameterInt>    ("firstStepIndex", "First Step Index", 0, MAX_NUM_OF_STEPS - 1, 0),
    std::make_unique<AudioParameterInt>    ("lastStepIndex", "Last Step Index", 0, MAX_NUM_OF_STEPS - 1, DEF_NUM_OF_STEPS - 1),
    std::make_unique<AudioParameterFloat>  ("swingValue", "Swing Value", SWING_MIN_VALUE, SWING_MAX_VALUE, SWING_MIN_VALUE),
    std::make_unique<AudioParameterInt>    ("basePitch", "Base Pitch", 36, 84, 60),
    std::make_unique<AudioParameterFloat>  ("portamento", "Portamento", 0.01f, 1.0f, 0.1f),
    std::make_unique<AudioParameterFloat>  ("lfoFrequency", "LFO Frequency",
                                           NormalisableRange<float>(0.01f, 10.0f, 0.001, 0.4),
                                           2.0f, "Hz"),
    std::make_unique<AudioParameterInt>    ("LfoLength", "LFO Length", 1, 32, 4, "Steps"),
    std::make_unique<AudioParameterFloat>  ("lfo2FMAmount", "LFO to FM Amount", 0.0f, 1.0f, 0.3f),
    std::make_unique<AudioParameterFloat>  ("lfo2ModMulti", "LFO to modulator multiplier", 0.0f, 1.0f, 0.0f),
    std::make_unique<AudioParameterFloat>  ("lfo2Panning", "LFO to panning", 0.0f, 1.0f, 0.0f),
    std::make_unique<AudioParameterChoice> ("lfoShape", "LFO Shape",LFO_SHAPES, 1),
    std::make_unique<AudioParameterFloat>  ("lfoPhase", "LFO Phase", 0.0f, 1.0f, 0.f),
    std::make_unique<AudioParameterChoice> ("lfoPolarity", "LFO Polarity", LFO_POLARITIES, 0),
    std::make_unique<AudioParameterBool>   ("lfoStepSync", "LFO Step Sync", false),
    std::make_unique<AudioParameterChoice> ("lfoRestart", "LFO Restart", LFO_RESTART_OPTIONS, 2),
    
    std::make_unique<AudioParameterFloat>  ("mod1Amount"     , "Mod 1 Amount"     , -1.0f, 1.0f, 0.f),
    std::make_unique<AudioParameterChoice> ("mod1Source"     , "Mod 1 Source"     , MODULATION_SOURCES, 0),
    std::make_unique<AudioParameterChoice> ("mod1Destination", "Mod 1 Destination", MODULATION_DESTINATIONS, 0),
    std::make_unique<AudioParameterFloat>  ("mod2Amount"     , "Mod 2 Amount"     , -1.0f, 1.0f, 0.f),
    std::make_unique<AudioParameterChoice> ("mod2Source"     , "Mod 2 Source"     , MODULATION_SOURCES, 0),
    std::make_unique<AudioParameterChoice> ("mod2Destination", "Mod 2 Destination", MODULATION_DESTINATIONS, 0),
    std::make_unique<AudioParameterFloat>  ("mod3Amount"     , "Mod 3 Amount"     , -1.0f, 1.0f, 0.f),
    std::make_unique<AudioParameterChoice> ("mod3Source"     , "Mod 3 Source"     , MODULATION_SOURCES, 0),
    std::make_unique<AudioParameterChoice> ("mod3Destination", "Mod 3 Destination", MODULATION_DESTINATIONS, 0),
    std::make_unique<AudioParameterFloat>  ("mod4Amount"     , "Mod 4 Amount"     , -1.0f, 1.0f, 0.f),
    std::make_unique<AudioParameterChoice> ("mod4Source"     , "Mod 4 Source"     , MODULATION_SOURCES, 0),
    std::make_unique<AudioParameterChoice> ("mod4Destination", "Mod 4 Destination", MODULATION_DESTINATIONS, 0),
    
    std::make_unique<AudioParameterFloat>  ("ampEnvAttack"     , "Amp Envelope Attack"      , 0.0f, 1.0f, 0.1f),
    std::make_unique<AudioParameterFloat>  ("ampEnvHold"       , "Amp Envelope Hold"        , 0.0f, 1.0f, 0.2f),
    std::make_unique<AudioParameterFloat>  ("ampEnvDecay"      , "Amp Envelope Decay"       , 0.0f, 1.0f, 0.4f),
    std::make_unique<AudioParameterFloat>  ("ampEnvLevel"      , "Amp Envelope Level"       , 0.0f, 1.0f, 1.0f),
    std::make_unique<AudioParameterFloat>  ("ampEnvAttackCurve", "Amp Envelope Attack Curve", 0.0f, 1.0f, 0.0f),
    std::make_unique<AudioParameterFloat>  ("ampEnvDecayCurve" , "Amp Envelope Decay Curve" , 0.0f, 1.0f, 0.7f),
    
    std::make_unique<AudioParameterFloat>  ("modEnvAttack"     , "Mod Envelope Attack"      , 0.0f, 1.0f, 0.0f),
    std::make_unique<AudioParameterFloat>  ("modEnvHold"       , "Mod Envelope Hold"        , 0.0f, 1.0f, 0.0f),
    std::make_unique<AudioParameterFloat>  ("modEnvDecay"      , "Mod Envelope Decay"       , 0.0f, 1.0f, 0.4f),
    std::make_unique<AudioParameterFloat>  ("modEnvLevel"      , "Mod Envelope Level"       , 0.0f, 1.0f, 0.5f),
    std::make_unique<AudioParameterFloat>  ("modEnvAttackCurve", "Mod Envelope Attack Curve", 0.0f, 1.0f, 0.0f),
    std::make_unique<AudioParameterFloat>  ("modEnvDecayCurve" , "Mod Envelope Decay Curve" , 0.0f, 1.0f, 0.0f),
    
    
    
    std::make_unique<AudioParameterInt>    ("step1Pitch",  "Step 1 Pitch",  -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step2Pitch",  "Step 2 Pitch",  -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step3Pitch",  "Step 3 Pitch",  -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step4Pitch",  "Step 4 Pitch",  -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step5Pitch",  "Step 5 Pitch",  -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step6Pitch",  "Step 6 Pitch",  -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step7Pitch",  "Step 7 Pitch",  -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step8Pitch",  "Step 8 Pitch",  -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step9Pitch",  "Step 9 Pitch",  -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step10Pitch", "Step 10 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step11Pitch", "Step 11 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step12Pitch", "Step 12 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step13Pitch", "Step 13 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step14Pitch", "Step 14 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step15Pitch", "Step 15 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step16Pitch", "Step 16 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step17Pitch", "Step 17 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step18Pitch", "Step 18 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step19Pitch", "Step 19 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step20Pitch", "Step 20 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step21Pitch", "Step 21 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step22Pitch", "Step 22 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step23Pitch", "Step 23 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step24Pitch", "Step 24 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step25Pitch", "Step 25 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step26Pitch", "Step 26 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step27Pitch", "Step 27 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step28Pitch", "Step 28 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step29Pitch", "Step 29 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step30Pitch", "Step 30 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step31Pitch", "Step 31 Pitch", -12, 12, 0),
    std::make_unique<AudioParameterInt>    ("step32Pitch", "Step 32 Pitch", -12, 12, 0),
    
})
{
    mStepperDataModel.reset ( new StepperSequencerDataModel() );
    addListener(&lfo);
    addListener(&sequencer);
    refreshEnvelopesModels();
    
    for (int stepIndex = 0; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
    {
        String parameterName {"step"};
        parameterName << stepIndex + 1 << "Pitch";
        mStepperDataModel->pitchValues.values[stepIndex] = mParameters.getRawParameterValue(parameterName);
    }
    
    
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
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FmseqnessAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FmseqnessAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
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
    sequencer.setSampleRate(sampleRate);
    sines.setSampleRate(sampleRate);
    lfo.setSampleRate(sampleRate);
    lfo.updateAngleDelta();
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
    
    for (auto sample = 0; sample < buffer.getNumSamples(); ++sample)
    {
        //Stops sound when sequencer is stopped
        if (isPlayingFloat->load() == 0.0f)
        {
            ampAhdEnv.startDecay();
            modAhdEnv.startDecay();
        }
        
        lfo.generate();
        
        if (sequencer.processToGetTrigger())
            trigger();
        
        portamentoCountDown = portamentoCountDown > 0 ? portamentoCountDown - 1 : 0;

        if (isNextStepGlide && portamentoCountDown == 0)
            poratmentoAccumulator += portamentoPitchUnit;

        ampEnv = ampAhdEnv.process(currentSampleRate);
        modEnv = modAhdEnv.process(currentSampleRate);
 
        LFOShape shape = LFOShape (int(lfoShape->load()));
        lfoAmp = lfo.getAmp(shape);
        
        processModMatrix(modEnv, lfoAmp, modSeqCurrentValue);
        
        float calculatedPitch    = currentStepPitch + poratmentoAccumulator + modMatrix.pitch * 36.0f;
        float calculatedFM       = currentStepFM + pow(modMatrix.fm * 2, 3);
        float calculatedModMulti = currentStepModMulti + modMatrix.modMulti * 2.0f;
        float calculatedVolume   = level * ampEnv * (modMatrix.volume + 1);
        float calculatedPanMod   = modMatrix.pan / 2.0f;
        
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
    
    for (int stepIndex = 0; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
    {
        String parameterName {"step"};
        parameterName << stepIndex + 1 << "Pitch";
        DBG(*mParameters.getRawParameterValue(parameterName));
    }
}

void FmseqnessAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr && xmlState->hasTagName (mParameters.state.getType()))
        mParameters.replaceState (ValueTree::fromXml (*xmlState));
    
    sendActionMessage ( "Preset Loaded" );
    
    for (int stepIndex = 0; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
    {
        String parameterName {"step"};
        parameterName << stepIndex + 1 << "Pitch";
        DBG(*mParameters.getRawParameterValue(parameterName));
    }
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
    currentStepPitch = mStepperDataModel->pitchValues.values[stepIndex]->load() + basePitch->load();
    targetPitch = getNextStepPitch();
    isNextStepGlide = getNextStepGlide();
    poratmentoAccumulator = 0.0f;
    
    auto gateState = mStepperDataModel->gateStateValues.values[stepIndex];
    
    int lfo2Restart = lfoRestart->load();
    
    if (lfo2Restart == 1)
        lfo.restart();
    
    if (gateState == StepGateState::off)
        return;
    
    if (gateState == StepGateState::on)
    {
        auto swingFactor = stepIndex % 2 == 0 ? swingValue->load() : 2.0f - swingValue->load();
        float stepLength = mStepperDataModel->getStepLength (stepIndex);
        
        if (int(stepLength) % 2 == 1)
            stepLength = stepLength -  1 + swingFactor;
        
        ampAhdEnv.reset (ampEnv, currentSampleRate, isNextStepGlide, stepLength);
        ampAhdEnv.state = PlayState::play;
        modAhdEnv.reset (ampEnv, currentSampleRate, isNextStepGlide, stepLength);
        modAhdEnv.state = PlayState::play;
        currentStepFM = mStepperDataModel->fmValues.values[stepIndex];
        currentStepFM = pow(currentStepFM * 2, 3);
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
    if (currentStep->load() == lastStepIndex->load())
        return mStepperDataModel->pitchValues.values[int(firstStepIndex->load())]->load() + basePitch->load();
    
    else if (currentStep->load() == MAX_NUM_OF_STEPS - 1)
        return mStepperDataModel->pitchValues.values[0]->load() + basePitch->load();

    return mStepperDataModel->pitchValues.values[int(currentStep->load()) + 1]->load() + basePitch->load();
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
        
        results[int(destination->load())] += sources[int(source->load())] * amount->load();
    }
    
    modMatrix.fm         = isnan(results[0]) ? 0.0f : results[0];
    modMatrix.pitch      = isnan(results[1]) ? 0.0f : results[1];
    modMatrix.pan        = isnan(results[2]) ? 0.0f : results[2];
    modMatrix.volume     = isnan(results[3]) ? 0.0f : results[3];
    modMatrix.modMulti   = isnan(results[4]) ? 0.0f : results[4];
    modMatrix.portamento = isnan(results[5]) ? 0.0f : results[5];
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
