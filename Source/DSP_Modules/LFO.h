/*
  ==============================================================================

    LFO.h
    Created: 20 May 2020 10:24:05am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Supporting_Files/Constants.h"
#include "../Supporting_Files/FMUtilities.h"

class LFO
{
public:
    LFO (AudioProcessorValueTreeState& parameters);
    ~LFO();
    
    void updateAngleDelta();
    void setSampleRate (double sampleRate);
    void setFrequency (double pitch);
    void restart();
    float generate();
    
private:
    AudioProcessorValueTreeState& mParameters;
    std::atomic<float>* frequency      { mParameters.getRawParameterValue ("lfoFrequency") };
    double currentAngle = 0.0, delta = 0.0;
    double currentSampleRate = 0.0;
    double normalizedCurrentFrequency = 0.0;
};
