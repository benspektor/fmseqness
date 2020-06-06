/*
  ==============================================================================

    StepSequencer.h
    Created: 6 Feb 2020 11:25:55pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "../Supporting_Files/Enums.h"
#include "../Supporting_Files/FMUtilities.h"
#include "../Supporting_Files/Constants.h"

class StepSequencer : public AudioProcessorListener
{
public:
    StepSequencer(AudioProcessorValueTreeState& parameters);
    ~StepSequencer();

    bool processToGetTrigger ( float swingValue );
    int getCurrentStepIndex();
    float getCurrentPitch();
    void setSampleRate ( double sampleRate );
    void updateStepDelte();
    void updateNumberOfSteps();
    
    //AudioProcessorListener
    void audioProcessorParameterChanged (AudioProcessor* processor,
                                         int parameterIndex,
                                         float newValue) override;
    void audioProcessorChanged (AudioProcessor* processor) override;
    
private:
    AudioProcessorValueTreeState& mParameters;
    
    std::atomic<float>* tempo           { mParameters.getRawParameterValue ("tempo") };
    std::atomic<float>* isPlaying       { mParameters.getRawParameterValue ("play") };
    std::atomic<float>* firstStepIndex  { mParameters.getRawParameterValue ("firstStepIndex") };
    std::atomic<float>* lastStepIndex   { mParameters.getRawParameterValue ("lastStepIndex") };
    
    float stepDelta = 0.0f;
    double currentSampleRate = 0.0;
    float ramp = 0.0;
    int prevStep = MAX_NUM_OF_STEPS;
    int currentNumOfSteps = 0;
    int currentStep = -1;
    int count = 0;
    int stepLengthInSamples = 0;
};
