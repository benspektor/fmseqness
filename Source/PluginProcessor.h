/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DataModels/StepperSequencerDataModel.h"
#include "DataModels/AHDEnvDataModel.h"
#include "DSP_Modules/FM2SineOscsGenerator.h"
#include "DSP_Modules/AHDEnv.h"
#include "DSP_Modules/StepSequencer.h"
#include "Supporting_Files/Constants.h"



//==============================================================================
/**
*/
class FmseqnessAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    FmseqnessAudioProcessor();
    ~FmseqnessAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void trigger();
    StepperSequencerDataModel& getStepperDataModel();
    AudioProcessorValueTreeState& getParametersTree();
    AHDEnvDataModel& getAmpAHDEnvDataModel();
    AHDEnvDataModel& getModAHDEnvDataModel();

private:
    AudioProcessorValueTreeState mParameters;
    AHDEnvDataModel mAmpAHDEnvModel, mModAHDEnvModel;
    AHDEnv ampAhdEnv { mParameters, mAmpAHDEnvModel }, modAhdEnv { mParameters, mModAHDEnvModel };
    StepSequencer seq { mParameters };
    FM2SineOscsGenerator sines { mParameters };
    std::unique_ptr<StepperSequencerDataModel> mStepperDataModel;
    double amp, mod = 0.0;
    double currentSampleRate = 0.0;
    std::atomic<float>* currentStep    { mParameters.getRawParameterValue("currentStep") };
    std::atomic<float>* numberOfSteps  { mParameters.getRawParameterValue("steps") };
    std::atomic<float>* isPlayingFloat { mParameters.getRawParameterValue("play") };
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmseqnessAudioProcessor)
};
