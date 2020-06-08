/*
  ==============================================================================

    FM2SineOscsGenerator.cpp
    Created: 7 Feb 2020 3:10:44pm
    Author:  Ben Spector

  ==============================================================================
*/

#include "FM2SineOscsGenerator.h"

FM2SineOscsGenerator::FM2SineOscsGenerator () {}
FM2SineOscsGenerator::~FM2SineOscsGenerator() {}

void FM2SineOscsGenerator::setSampleRate (double sampleRate)
{
    currentSampleRate = sampleRate;
}

void FM2SineOscsGenerator::restart()
{
    currentCarrierRamp   = 0.0f;
    currentModulatorRamp = 0.0f;
}

float FM2SineOscsGenerator::generate(float pitch, float fmAmount, float modMulti)
{
    if (currentSampleRate == 0.0)
        return 0.0f;
    
    if (currentModulatorRamp >= 1)
        currentModulatorRamp -= 1;
    
    if (currentCarrierRamp >= 1)
        currentCarrierRamp -= 1;
    
    auto hzValue             = 440.0f * std::pow (2.0, (pitch - 69) / 12.0);
    auto carrierAngleDelta   = hzValue * 2 / currentSampleRate;
    auto modulatorAngleDelta = carrierAngleDelta * modMulti;
    
    fmAmount = jmax (fmAmount, 0.0f);
    
    currentModulatorRamp += modulatorAngleDelta;
    currentCarrierRamp   += carrierAngleDelta + std::sin (currentModulatorRamp * 2 * MathConstants<float>::pi) * fmAmount;
    
    return std::sin (currentCarrierRamp * 2 * MathConstants<float>::pi);;
}

