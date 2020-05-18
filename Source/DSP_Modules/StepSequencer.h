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

class StepSequencer
{
public:
    StepSequencer(AudioProcessorValueTreeState& parameters);
    ~StepSequencer();

    bool processToGetTrigger (double currentSampleRate);
    int getCurrentStepIndex();
    float getCurrentPitch();
    
private:
    AudioProcessorValueTreeState& mParameters;
    
    std::atomic<float>* tempo           { mParameters.getRawParameterValue ("tempo") };
    std::atomic<float>* isPlayingFloat  { mParameters.getRawParameterValue ("play") };
    std::atomic<float>* numOfStepsFloat { mParameters.getRawParameterValue ("steps") };
    std::atomic<float>* firstStepIndex  { mParameters.getRawParameterValue ("firstStepIndex") };
    std::atomic<float>* lastStepIndex   { mParameters.getRawParameterValue ("lastStepIndex") };
    std::atomic<float>* swingValue      { mParameters.getRawParameterValue ("swingValue") };
    
    float ramp = 0.0;
    int prevStep = MAX_NUM_OF_STEPS;
    int currentNumOfSteps = 0;
    int currentStep = 0;
};
