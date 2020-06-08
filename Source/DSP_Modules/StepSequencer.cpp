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

bool StepSequencer::processToGetTrigger ( float swingValue )
{
    
    bool isTrigger = false;
    
    
    if ( ! *isPlaying)
    {
        currentStep = -1;
        samplesCount = 0;
        return false;
    }

    
    if (currentStep % 2 == 0)
    {
        if (samplesCount >= stepLengthInSamples * swingValue)
            samplesCount = 0;
    }
    else
    {
        if (samplesCount >= stepLengthInSamples * (2.0f - swingValue))
            samplesCount = 0;
    }
    
    if (samplesCount == 0)
    {
        isTrigger = true;
        currentStep++;
    }
    
    samplesCount++;
    
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

bool StepSequencer::isCurrentStepSwinged()
{
    return currentStep % 2 == 1;
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
