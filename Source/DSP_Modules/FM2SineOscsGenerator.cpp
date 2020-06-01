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

float FM2SineOscsGenerator::generate(float pitch, float fmAmount, float modMulti)
{
    if (currentSampleRate == 0.0)
        return 0.0f;
    
    auto hzValue = 440.0f * std::pow (2.0, (pitch - 69) / 12.0);
    auto normalizedCurrentCarrierFrequency = hzValue * 2 / currentSampleRate;
    
    auto carrierAngleDelta   = normalizedCurrentCarrierFrequency; // * MathConstants<float>::pi;
    auto modulatorAngleDelta = normalizedCurrentCarrierFrequency * modMulti; // * MathConstants<float>::pi;
    
    
    
    
    currentModulatorRamp += modulatorAngleDelta;
    
    if (currentModulatorRamp >= 1)
        currentModulatorRamp -= 1;
    
    auto modulatorSine = std::sin (currentModulatorRamp * 2 * MathConstants<float>::pi);
        
    fmAmount = jmax (fmAmount, 0.0f);
    
    currentCarrierRamp += carrierAngleDelta + modulatorSine * fmAmount;
    
    if (currentCarrierRamp >= 1)
        currentCarrierRamp -= 1;
    
    auto carrierSine =   std::sin (currentCarrierRamp * 2 * MathConstants<float>::pi);
    return carrierSine;
}

