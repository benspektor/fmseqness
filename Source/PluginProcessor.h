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
    int getNumberOfSamplesInStep();
    float getNextStepPitch();
    bool getNextStepGlide();
//    float calculateCurrentPitch();
    StepperSequencerDataModel& getStepperDataModel();
    AudioProcessorValueTreeState& getParametersTree();
    AHDEnvDataModel& getAmpAHDEnvDataModel();
    AHDEnvDataModel& getModAHDEnvDataModel();

    float portamentoPitchUnit = 0.0f;
    float pitch = 0;
    float targetPitch = 0;
    int portamentoCountDown = 0;
private:
    AudioProcessorValueTreeState mParameters;
    AHDEnvDataModel mAmpAHDEnvModel, mModAHDEnvModel;
    AHDEnv ampAhdEnv { mParameters, mAmpAHDEnvModel }, modAhdEnv { mParameters, mModAHDEnvModel };
    StepSequencer seq { mParameters };
    FM2SineOscsGenerator sines { mParameters };
    std::unique_ptr<StepperSequencerDataModel> mStepperDataModel;
    double amp = 0.0, mod = 0.0;
    double currentSampleRate = 0.0;
    float currentStepLevel = 0.0f;
    bool isNextStepGlide = false;
    bool isPortamentoActive = false;
    
    
    std::atomic<float>* currentStep    { mParameters.getRawParameterValue ("currentStep") };
    std::atomic<float>* numberOfSteps  { mParameters.getRawParameterValue ("steps") };
    std::atomic<float>* isPlayingFloat { mParameters.getRawParameterValue ("play") };
    std::atomic<float>* basePitch      { mParameters.getRawParameterValue ("basePitch") };
    std::atomic<float>* tempo          { mParameters.getRawParameterValue ("tempo") };
    std::atomic<float>* portamento     { mParameters.getRawParameterValue ("portamento") };
    std::atomic<float>* firstStepIndex { mParameters.getRawParameterValue ("firstStepIndex") };
    std::atomic<float>* lastStepIndex  { mParameters.getRawParameterValue ("lastStepIndex") };
    std::atomic<float>* swingValue     { mParameters.getRawParameterValue ("swingValue") };
    
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmseqnessAudioProcessor)
};
