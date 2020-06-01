/*
  ==============================================================================

    FM2SineOscsGenerator.h
    Created: 7 Feb 2020 3:10:44pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Supporting_Files/Constants.h"
#include "../Supporting_Files/FMUtilities.h"


class FM2SineOscsGenerator
{
public:
    FM2SineOscsGenerator();
    ~FM2SineOscsGenerator();
    void setSampleRate (double sampleRate);
    float generate(float pitch, float fmAmount, float modMulti);

private:
    float currentCarrierRamp = 0.0;
    float currentModulatorRamp = 0.0;
    double currentSampleRate = 0.0;
};
