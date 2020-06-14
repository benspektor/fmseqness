/*
  ==============================================================================

    FMUtilities.h
    Created: 6 Feb 2020 10:28:27am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class FMUtilities {
    
public:
    static double convertTimeToFrequency (double timeInSeconds, double sampleRate)
    {
        return 1 / (timeInSeconds * sampleRate);
    }
    
    static float convert0to1toCurveValue (float value)
    {
        return value <= 0.5 ? value + 0.5 : 1.0 + (value - 0.5) * 14.0;
    }
    
    static float convertPitchToHz (float pitch)
    {
        return 440.0f * std::pow (2.0, (pitch - 69) / 12.0);
    }
};


