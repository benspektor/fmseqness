/*
  ==============================================================================

    StepSequencer.cpp
    Created: 6 Feb 2020 11:25:55pm
    Author:  Ben Spector

  ==============================================================================
*/

#include "StepSequencer.h"

StepSequencer::StepSequencer(AudioProcessorValueTreeState& parameters) : mParameters(parameters) {}
StepSequencer::~StepSequencer() {}

bool StepSequencer::processToGetTrigger (double currentSampleRate)
{
    bool isPlaying = isPlayingFloat->load();
    int numOfSteps = numOfStepsFloat->load();
    
    if (isPlaying == false)
    {
        ramp = numOfSteps;
        currentStep = numOfSteps;
        return false;
    }
    
    auto stepDelta = tempo->load() / 60*4 / currentSampleRate;
    ramp = ramp < numOfSteps ? ramp + stepDelta : 0.0;
    ramp = ramp < numOfSteps ? ramp : 0.0;
    currentStep = ramp;
    
    if (prevStep != currentStep)
    {
        prevStep = currentStep;
        return true;
    }
    else
    {
        return false;
    }
}

int StepSequencer::getCurrentStepIndex()
{
    return currentStep;
}
