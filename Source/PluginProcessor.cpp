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
    std::make_unique<AudioParameterFloat> ("globalFMAmount", "Global FM Amount",
                                           NormalisableRange<float>(FM_AMNT_MIN_VALUE, FM_AMNT_MAX_VALUE, 0.001, FM_AMNT_SKEW_VALUE),
                                           FM_AMNT_SET_VALUE),
    std::make_unique<AudioParameterFloat> ("modulatorMultiplier", "Modulator Multiplier",
                                           NormalisableRange<float>(MOD_MULTI_MIN_VALUE, MOD_MULTI_MAX_VALUE, 0.001, MOD_MULTI_SKEW_VALUE),
                                           MOD_MULTI_SET_VALUE),
    std::make_unique<AudioParameterFloat> ("tempo", "Tempo",
                                           NormalisableRange<float>(TEMPO_MIN_VALUE, TEMPO_MAX_VALUE, 1.0, TEMPO_SKEW_VALUE),
                                           TEMPO_SET_VALUE),
    std::make_unique<AudioParameterInt>   ("steps", "Steps", MIN_NUM_OF_STEPS, MAX_NUM_OF_STEPS, DEF_NUM_OF_STEPS),
    std::make_unique<AudioParameterBool>  ("play", "Play", false),
    std::make_unique<AudioParameterFloat> ("currentStep", "Current Step", NormalisableRange<float>(0.0, MAX_NUM_OF_STEPS, 1.0, 1.0), MAX_NUM_OF_STEPS),
    std::make_unique<AudioParameterInt>   ("firstStepIndex", "First Step Index", 0, MAX_NUM_OF_STEPS - 1, 0),
    std::make_unique<AudioParameterInt>   ("lastStepIndex", "Last Step Index", 0, MAX_NUM_OF_STEPS - 1, DEF_NUM_OF_STEPS - 1),
    std::make_unique<AudioParameterFloat> ("swingValue", "Swing Value", SWING_MIN_VALUE, SWING_MAX_VALUE, SWING_MIN_VALUE),
    std::make_unique<AudioParameterInt>   ("basePitch", "Base Pitch", 36, 84, 60),
    std::make_unique<AudioParameterFloat> ("portamento", "Portamento", 0.01f, 1.0f, 0.1f),
    std::make_unique<AudioParameterFloat> ("lfoFrequency", "LFO Frequency",
                                           NormalisableRange<float>(0.1f, 10.0f, 0.001, 1.0),
                                           2.0f)
})
{
    mStepperDataModel.reset ( new StepperSequencerDataModel() );
    mAmpAHDEnvModel.attack = 0.02;
    mAmpAHDEnvModel.attackCurve = 0.5;
    mAmpAHDEnvModel.hold   = 0.4;
    mAmpAHDEnvModel.decay  = 0.3;
    mAmpAHDEnvModel.decayCurve = 4.0;
    mAmpAHDEnvModel.level  = 1.0;
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
    sines.setSampleRate(sampleRate);
    sines.updateAngleDelta();
    lfo.setSampleRate(sampleRate);
    lfo.setFrequency(1.0);
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
    
    auto level = 0.250f;
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
        
        if (sequencer.processToGetTrigger(currentSampleRate))
            trigger();
        
//        portamentoCountDown = portamentoCountDown > 0 ? portamentoCountDown - 1 : 0;
//
//        if (isNextStepGlide && portamentoCountDown == 0)
//        {
//
//            float distance = pitch - targetPitch;
//
//            if (distance != 0.0)
//            {
//                if ( abs(distance) < abs(portamentoPitchUnit) || distance * portamentoPitchUnit > 0)
//                {
//                    pitch = targetPitch;
//                    sines.setCurrentPitch(pitch);
//                    sines.updateAngleDelta();
//                }
//                else
//                {
//                    pitch += portamentoPitchUnit;
//                    sines.setCurrentPitch(pitch);
//                    sines.updateAngleDelta();
//                }
//            }
//
//
//        }

        lfoAmp = lfo.generate();
        
        
        
        amp = ampAhdEnv.process(currentSampleRate);
        mod = modAhdEnv.process(currentSampleRate);

        auto currentSample = sines.generate(mod * mod + lfoAmp * 0.1) * amp * level * currentStepLevel;
        
        leftBuffer[sample]  = currentSample;
        rightBuffer[sample] = currentSample;
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

void FmseqnessAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<XmlElement> xmlState (getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr && xmlState->hasTagName (mParameters.state.getType()))
        mParameters.replaceState (ValueTree::fromXml (*xmlState));
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
    targetPitch = getNextStepPitch();
    isNextStepGlide = getNextStepGlide();

    
    auto gateState = mStepperDataModel->gateStateValues.values[stepIndex];

    if (gateState == StepGateState::off)
        return;
    
    
    
    if (gateState == StepGateState::on)
    {
        

        auto swingFactor = stepIndex % 2 == 0 ? swingValue->load() : 2.0f - swingValue->load();
        float stepLength = mStepperDataModel->getStepLength (stepIndex);
        
        if (int(stepLength) % 2 == 1)
            stepLength = stepLength -  1 + swingFactor;
        
        ampAhdEnv.reset (amp, currentSampleRate, isNextStepGlide, stepLength);
        ampAhdEnv.state = PlayState::play;
        modAhdEnv.reset (amp, currentSampleRate, isNextStepGlide, stepLength);
        modAhdEnv.state = PlayState::play;
        currentStepLevel = mStepperDataModel->levelValues.values[int(currentStep->load())];
        const float StepFMModValue = mStepperDataModel->modValues.values[stepIndex];
        sines.setStepFMModMulti(StepFMModValue);
    }
    
    if (pitch != mStepperDataModel->pitchValues.values[stepIndex] + basePitch->load())
    {
//        DBG(mStepperDataModel->pitchValues.values[stepIndex] + basePitch->load());
//        DBG(pitch);
    }
    
    pitch = mStepperDataModel->pitchValues.values[stepIndex] + basePitch->load();
    sines.setCurrentPitch(pitch);
    sines.updateAngleDelta();
    
//    portamentoPitchUnit =  1.1 * (targetPitch - pitch ) / (portamento->load() * getNumberOfSamplesInStep());
//    portamentoPitchUnit = isNextStepGlide ? portamentoPitchUnit : 0.0f;
//    portamentoCountDown = getNumberOfSamplesInStep() * (1.0 - portamento->load());
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
        return mStepperDataModel->pitchValues.values[int(firstStepIndex->load())] + basePitch->load();
    
    else if (currentStep->load() == MAX_NUM_OF_STEPS - 1)
        return mStepperDataModel->pitchValues.values[0] + basePitch->load();

    return mStepperDataModel->pitchValues.values[int(currentStep->load()) + 1] + basePitch->load();
}

bool FmseqnessAudioProcessor::getNextStepGlide()
{
    if (currentStep->load() == lastStepIndex->load())
        return mStepperDataModel->gateStateValues.values[int(firstStepIndex->load())];
    
    else if (currentStep->load() == MAX_NUM_OF_STEPS - 1)
        return mStepperDataModel->gateStateValues.values[0] + basePitch->load();
    
    return mStepperDataModel->gateStateValues.values[int(currentStep->load()) + 1];
}
