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
    
    currentNumOfSteps = firstStepIndex->load() <= lastStepIndex->load() ?
                        lastStepIndex->load() - firstStepIndex->load() + 1 :
                        MAX_NUM_OF_STEPS - firstStepIndex->load() + lastStepIndex->load() + 1;
    
    if (isPlaying == false)
    {
        ramp = 0.0;
        currentStep = 0;
        return false;
    }
    
    auto stepDelta = tempo->load() / 60*8 / currentSampleRate;
    
    ramp = ramp + stepDelta;
 
    bool isNextStepSwingCandidate = currentStep % 2 == 0;
    
    if (isNextStepSwingCandidate)
    {
        bool isNextStepSwingArrived = ramp >= currentStep + swingValue->load();
        
        if (isNextStepSwingArrived)
            currentStep = ramp;
    }
    else
    {
        currentStep = ramp;
    }
    
    
    
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
//    return int (currentStep + firstStepIndex->load()) % MAX_NUM_OF_STEPS;
    return int (currentStep % currentNumOfSteps + firstStepIndex->load()) % MAX_NUM_OF_STEPS;
}
