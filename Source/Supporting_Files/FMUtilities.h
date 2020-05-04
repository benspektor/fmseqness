/*
  ==============================================================================

    FMUtilities.h
    Created: 6 Feb 2020 10:28:27am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

class FMUtilities {
    
public:
    static double convertTimeToFrequency (double timeInSeconds, double sampleRate)
    {
        return 1 / timeInSeconds * 2 / sampleRate;
    }
};


