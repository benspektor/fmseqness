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
    
    delta = normalizedCurrentFrequency * MathConstants<double>::pi;
}


void LFO::setSampleRate (double sampleRate)
{
    currentSampleRate = sampleRate;
}

void LFO::setFrequency (double frequency)
{
    normalizedCurrentFrequency = frequency / currentSampleRate;
}

float LFO::generate()
{
    auto sine = (float) std::sin (currentAngle);
    currentAngle += delta;
    return sine;
}

void LFO::restart()
{
    currentAngle = 0.0;
}


