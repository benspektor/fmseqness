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
                                           NormalisableRange<float>(TEMPO_MIN_VALUE, TEMPO_MAX_VALUE, 0.01, TEMPO_SKEW_VALUE),
                                           TEMPO_SET_VALUE),
    std::make_unique<AudioParameterInt> ("steps", "Steps", MIN_NUM_OF_STEPS, MAX_NUM_OF_STEPS, DEF_NUM_OF_STEPS),
    std::make_unique<AudioParameterBool> ("play", "Play", false),
    std::make_unique<AudioParameterFloat> ("currentStep", "Current Step", NormalisableRange<float>(0.0, MAX_NUM_OF_STEPS, 1.0, 1.0), MAX_NUM_OF_STEPS),
    std::make_unique<AudioParameterInt> ("firstStepIndex", "First Step Index", 0, MAX_NUM_OF_STEPS - 1, 0),
    std::make_unique<AudioParameterInt> ("lastStepIndex", "Last Step Index", 0, MAX_NUM_OF_STEPS - 1, DEF_NUM_OF_STEPS - 1)
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
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    
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
        
        if (seq.processToGetTrigger(currentSampleRate))
            trigger();
        
        amp = ampAhdEnv.process(currentSampleRate);
        mod = modAhdEnv.process(currentSampleRate);

        auto currentSample = sines.generate(mod) * amp * level * currentStepLevel;
        
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
    const int stepIndex = seq.getCurrentStepIndex();
    currentStep->store(stepIndex);

    if (mStepperDataModel->gateStateValues.values[stepIndex] == StepGateState::off)
        return;
    
    const float pitch = mStepperDataModel->pitchValues.values[stepIndex] + BASE_PITCH;
    const float StepFMModValue = mStepperDataModel->modValues.values[stepIndex];
    
    sines.setCurrentPitch(pitch);
    sines.setStepFMModMulti(StepFMModValue);
    sines.updateAngleDelta();
    
    if (mStepperDataModel->gateStateValues.values[stepIndex] == StepGateState::glide)
        return;
    
    auto isNextStepGlide = stepIndex == numberOfSteps->load() - 1 ? false : mStepperDataModel->isNextStepGlide(stepIndex);
    
    ampAhdEnv.reset (amp, currentSampleRate, isNextStepGlide);
    ampAhdEnv.state = PlayState::play;
    modAhdEnv.reset (amp, currentSampleRate, false);
    modAhdEnv.state = PlayState::play;
    currentStepLevel = mStepperDataModel->levelValues.values[int(currentStep->load())];
}

AHDEnvDataModel& FmseqnessAudioProcessor::getAmpAHDEnvDataModel()
{
    return mAmpAHDEnvModel;
}

AHDEnvDataModel& FmseqnessAudioProcessor::getModAHDEnvDataModel()
{
    return mModAHDEnvModel;
}
