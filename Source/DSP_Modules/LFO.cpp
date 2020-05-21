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
    setFrequency(frequency->load());
    updateAngleDelta();
}
LFO::~LFO() {}

void LFO::updateAngleDelta()
{
    if (currentSampleRate == 0.0)
        return;
    
    float stepLength = tempo->load() / (60 * 8);
    float totalLength = stepLength * length->load();
    float frequency = 1 / totalLength;
    
    normalizedCurrentFrequency =  frequency * 2 / currentSampleRate;
    
    delta = normalizedCurrentFrequency; //* MathConstants<double>::pi;
}


void LFO::setSampleRate (double sampleRate)
{
    currentSampleRate = sampleRate;
}

void LFO::setFrequency (double frequency)
{
//    normalizedCurrentFrequency = frequency / currentSampleRate;
}

float LFO::generate()
{
    auto sine = (float) std::sin (currentAngle);
    currentAngle = currentAngle >= 1 ? -1 : currentAngle;
    auto out = currentAngle;
    currentAngle += delta;
    return out;
}

void LFO::restart()
{
    currentAngle = 0.0;
}

void LFO::audioProcessorParameterChanged (AudioProcessor* processor,
                                     int parameterIndex,
                                     float newValue)
{
    switch (parameterIndex)
    {
        // LFO length
        case 12:
            updateAngleDelta();
            break;
        default:
            break;
    }
}

void LFO::audioProcessorChanged (AudioProcessor* processor) {}


