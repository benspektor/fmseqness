/*
  ==============================================================================

    StepSequencer.cpp
    Created: 6 Feb 2020 11:25:55pm
    Author:  Ben Spector

  ==============================================================================
*/

#include "StepSequencer.h"

StepSequencer::StepSequencer(AudioProcessorValueTreeState& parameters) : mParameters(parameters)
{
    updateNumberOfSteps();
}
StepSequencer::~StepSequencer() {}

bool StepSequencer::processToGetTrigger ()
{
    bool isPlaying = isPlayingFloat->load();
    bool isTrigger = false;
    
    if (isPlaying == false)
    {
        ramp = 0.0;
        currentStep = -1;
        count = 0;
        return false;
    }
    
//    bool isNextStepSwingCandidate = currentStep % 2 == 0;
//
//    if (isNextStepSwingCandidate)
//    {
//        bool isNextStepSwingArrived = ramp >= currentStep + swingValue->load();
//
//        if (isNextStepSwingArrived)
//            currentStep = ramp;
//    }
//    else
//    {
//        currentStep = ramp;
//    }
    
//    currentStep = ramp;
//
//    if (prevStep != currentStep)
//    {
//        prevStep = currentStep;
//        isTrigger = true;
//        DBG(count);
//        count = 0;
//    }
    
    
    
//    ramp = ramp + stepDelta;
    
//    bool isNextStepSwingCandidate = currentStep % 2 == 0;
//
//    if (isNextStepSwingCandidate)
//    {
//        bool isNextStepSwingArrived = count > stepLengthInSamples * swingValue->load();
//
//        if (isNextStepSwingArrived)
//        {
//            DBG(count);
//            currentStep++;
//            isTrigger = true;
//            count -= stepLengthInSamples;
//        }
//    }
//    else if (count > stepLengthInSamples)
//    {
//        DBG(count);
//        count -= stepLengthInSamples;
//        currentStep++;
//        isTrigger = true;
//    }
    
    if (currentStep % 2 == 0)
    {
        if (count >= stepLengthInSamples * swingValue->load())
            count = 0;
    }
    else
    {
        if (count >= stepLengthInSamples * (2.0f - swingValue->load()))
            count = 0;
    }
    
    if (count == 0)
    {
        isTrigger = true;
        currentStep++;
    }
    
    count++;
    return isTrigger;
}

int StepSequencer::getCurrentStepIndex()
{
    return int (currentStep % currentNumOfSteps + firstStepIndex->load()) % MAX_NUM_OF_STEPS;
}

void StepSequencer::setSampleRate (double sampleRate)
{
    this->currentSampleRate = sampleRate;
    updateStepDelte();
}

void StepSequencer::updateStepDelte()
{
    if (currentSampleRate == 0.0)
        return;

    stepDelta = tempo->load() / 60 * 4 / currentSampleRate;
    stepLengthInSamples = 1 / stepDelta;
}

void StepSequencer::updateNumberOfSteps()
{
    currentNumOfSteps = firstStepIndex->load() <= lastStepIndex->load() ?
                        lastStepIndex->load() - firstStepIndex->load() + 1 :
                        MAX_NUM_OF_STEPS - firstStepIndex->load() + lastStepIndex->load() + 1;
}

void StepSequencer::audioProcessorParameterChanged (AudioProcessor* processor,
                                     int parameterIndex,
                                     float newValue)
{
    switch (parameterIndex)
    {
        //Tempo
        case 2:
            updateStepDelte();
            break;
        default:
            break;
    }
}

void StepSequencer::audioProcessorChanged (AudioProcessor* processor) {}
