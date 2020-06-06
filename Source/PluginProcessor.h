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
#include "DataModels/ModMatrixDataModel.h"
#include "DSP_Modules/FM2SineOscsGenerator.h"
#include "DSP_Modules/AHDEnv.h"
#include "DSP_Modules/StepSequencer.h"
#include "DSP_Modules/LFO.h"
#include "Supporting_Files/SupportingFiles.h"




//==============================================================================
/**
*/
class FmseqnessAudioProcessor  : public AudioProcessor, public ActionBroadcaster
{
public:
    struct ModMatrixProcessResults
    {
        float fm;
        float pitch;
        float pan;
        float volume;
        float modMulti;
        float swing;
        
        void resetAll()
        {
            fm         = 0.0f;
            pitch      = 0.0f;
            pan        = 0.0f;
            volume     = 0.0f;
            modMulti   = 0.0f;
            swing      = 0.0f;
        }
    };
    void extracted();
    
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
    float getLfoAmp();
    void updateSequncerNumberOfSteps();
    void updateLFOAngle();
    float getModulatorMultiFrom01 (float value);
    void processModMatrix(float env, float lfo, float modSeq);
    void refreshEnvelopesModels();
    void matrixModelSetup();
    StepperSequencerDataModel& getStepperDataModel();
    AudioProcessorValueTreeState& getParametersTree();
    AHDEnvDataModel& getAmpAHDEnvDataModel();
    AHDEnvDataModel& getModAHDEnvDataModel();
    ModMatrixProcessResults modMatrix;
    float ampEnv = 0.0, modEnv = 0.0, modSeqCurrentValue = 0.0;
private:
    AudioProcessorValueTreeState mParameters;
    AHDEnvDataModel mAmpAHDEnvModel, mModAHDEnvModel;
    AHDEnv ampAhdEnv { mParameters, mAmpAHDEnvModel }, modAhdEnv { mParameters, mModAHDEnvModel };
    StepSequencer sequencer { mParameters };
    FM2SineOscsGenerator sines;
    LFO lfo { mParameters };
    std::unique_ptr<StepperSequencerDataModel> mStepperDataModel;
    ModMatrixModel matrix;
    
    double currentSampleRate = 0.0;
   
    bool isNextStepGlide = false;
    float portamentoPitchUnit = 0.0f;
    float currentStepPitch = 0;
    float currentStepFM = 0.0f;
    float currentStepModMulti = 0.0f;
    float targetPitch = 0;
    float poratmentoAccumulator = 0.0f;
    int portamentoCountDown = 0;
    float lfoAmp = 0.0;
    float currentSwingValue = 0.0f;
   
    
    
    std::atomic<float>* currentStep    { mParameters.getRawParameterValue ("currentStep") };
    std::atomic<float>* numberOfSteps  { mParameters.getRawParameterValue ("steps") };
    std::atomic<float>* isPlayingFloat { mParameters.getRawParameterValue ("play") };
    std::atomic<float>* basePitch      { mParameters.getRawParameterValue ("basePitch") };
    std::atomic<float>* tempo          { mParameters.getRawParameterValue ("tempo") };
    std::atomic<float>* portamento     { mParameters.getRawParameterValue ("portamento") };
    std::atomic<float>* firstStepIndex { mParameters.getRawParameterValue ("firstStepIndex") };
    std::atomic<float>* lastStepIndex  { mParameters.getRawParameterValue ("lastStepIndex") };
    std::atomic<float>* swingValue     { mParameters.getRawParameterValue ("swingValue") };
    std::atomic<float>* globalFMAmount { mParameters.getRawParameterValue ("globalFMAmount") };
    std::atomic<float>* lfo2Panning    { mParameters.getRawParameterValue ("lfo2Panning") };
    std::atomic<float>* lfo2FMAmount   { mParameters.getRawParameterValue ("lfo2FMAmount") };
    std::atomic<float>* lfo2ModMulti   { mParameters.getRawParameterValue ("lfo2ModMulti") };
    std::atomic<float>* lfoShape       { mParameters.getRawParameterValue ("lfoShape") };
    std::atomic<float>* fmAmount       { mParameters.getRawParameterValue ("globalFMAmount") };
    std::atomic<float>* modulatorMulti { mParameters.getRawParameterValue ("modulatorMultiplier") };
    std::atomic<float>* lfoRestart     { mParameters.getRawParameterValue ("lfoRestart") };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmseqnessAudioProcessor)
};
