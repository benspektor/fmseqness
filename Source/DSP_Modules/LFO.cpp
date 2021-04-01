/*
  ==============================================================================

    LFO.cpp
    Created: 20 May 2020 10:24:05am
    Author:  Ben Spector

  ==============================================================================
*/

#include "LFO.h"



LFO::LFO (AudioProcessorValueTreeState& parameters) : mParameters (parameters)
{
    updateAngleDelta();
    updatePolarity() ;
}
LFO::~LFO() {}

void LFO::updateAngleDelta()
{
    if (currentSampleRate == 0.0)
        return;
    
    bool isStepSync = stepSync->load();
    float currentFrequency = 0.0f;
    
    if (isStepSync)
    {
        float stepLength = tempo->load() / (60 * 4);
        float totalLength = stepLength * length->load();
        currentFrequency = 2 / totalLength;
    }
    else
    {
        currentFrequency = frequency->load();
    }
    
    delta =  currentFrequency * 2 / currentSampleRate;
}


void LFO::setSampleRate (double sampleRate)
{
    currentSampleRate = sampleRate;
    updateAngleDelta();
}


void LFO::restart()
{
    amp = phase->load();
}

float LFO::generate (LFOShape shape)
{
    amp = amp >= 1 ? amp - 1 : amp + delta;
    
    switch (shape)
    {
        case LFOShape::saw:
            return isUni ? amp * direction : (amp * 2) - 1 ;
            break;
            
        case LFOShape::sine:
            return isUni ? sin ( amp * MathConstants<float>::pi ) * direction : sin ( amp * 2 * MathConstants<float>::pi );
            break;
            
        case LFOShape::triangle:
            return isUni ? abs(amp * 2 - 1) * direction : abs(amp * 2 - 1) * 2 - 1;
            break;
            
        case LFOShape::square:
            return isUni ? (amp <= 0.5 ? 0 : 1) * direction : (amp <= 0.5 ? -1 : 1);
        default:
            break;
    }
}

void LFO::updatePolarity()
{
    isUni = polarity->load() != 0;
    direction = polarity->load() == 2 ? -1 : 1;
}


void LFO::audioProcessorParameterChanged (AudioProcessor* processor,
                                     int parameterIndex,
                                     float newValue)
{
    switch (parameterIndex)
    {
        // LFO Freqeuncy
        case 11:
            updateAngleDelta();
            break;
        // LFO length
        case 12:
            updateAngleDelta();
            break;
        // LFO polarity
        case 18:
            updatePolarity();
            break;
        // LFO stepSync
        case 19:
            updateAngleDelta();
            break;
        default:
            break;
    }
}

void LFO::audioProcessorChanged (AudioProcessor* processor, const ChangeDetails& details) {}


