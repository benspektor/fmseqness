/*
  ==============================================================================

    FM2SineOscsGenerator.cpp
    Created: 7 Feb 2020 3:10:44pm
    Author:  Ben Spector

  ==============================================================================
*/

#include "FM2SineOscsGenerator.h"

FM2SineOscsGenerator::FM2SineOscsGenerator (AudioProcessorValueTreeState& parameters) : mParameters (parameters) {}
FM2SineOscsGenerator::~FM2SineOscsGenerator() {}

void FM2SineOscsGenerator::updateAngleDelta()
{
    if (currentSampleRate == 0.0)
        return;
    
    float modMulti = modulatorMulti->load() * modulatorMultiMod;
    
    carrierAngleDelta = normalizedCurrentCarrierFrequency * MathConstants<double>::pi;
    modulatorAngleDelta = normalizedCurrentCarrierFrequency * modMulti * MathConstants<double>::pi;
}

void FM2SineOscsGenerator::setStepFMModMulti (double factor)
{
    stepFMModMulti = factor;
    updateAngleDelta();
}

void FM2SineOscsGenerator::setSampleRate (double sampleRate)
{
    currentSampleRate = sampleRate;
}

void FM2SineOscsGenerator::setCurrentPitch (double pitch)
{
    auto hzValue = MidiMessage::getMidiNoteInHertz (pitch);
    normalizedCurrentCarrierFrequency = hzValue * 2 / currentSampleRate;
}

float FM2SineOscsGenerator::generate(float externalModulationAmount)
{
    if (currentSampleRate == 0.0)
        return 0.0f;
    
    
    auto modulatorSine = (float) std::sin (currentModulatorAngle);
    auto carrierSine = (float) std::sin (currentCarrierAngle);
    currentModulatorAngle += modulatorAngleDelta;
    const float mod = isnan(externalModulationAmount) ? 0 : externalModulationAmount;
    auto modulationAmount = fmAmount->load() + stepFMModMulti * mod;
    modulationAmount = jmax (modulationAmount, 0.0);
    currentCarrierAngle += carrierAngleDelta + modulatorSine * modulationAmount;
    return carrierSine;
}

float FM2SineOscsGenerator::generate(float pitch, float fmAmount, float modMulti)
{
    if (currentSampleRate == 0.0)
        return 0.0f;
    
    auto hzValue = MidiMessage::getMidiNoteInHertz (pitch);
    normalizedCurrentCarrierFrequency = hzValue * 2 / currentSampleRate;
    
    carrierAngleDelta   = normalizedCurrentCarrierFrequency * MathConstants<double>::pi;
    modulatorAngleDelta = normalizedCurrentCarrierFrequency * modMulti * MathConstants<double>::pi;
    
    auto modulatorSine = (float) std::sin (currentModulatorAngle);
    auto carrierSine =   (float) std::sin (currentCarrierAngle);
    
    currentModulatorAngle += modulatorAngleDelta;
    
    float modulationAmount = isnan(fmAmount) ? 0 : fmAmount;
    
    modulationAmount = jmax (modulationAmount, 0.0f);
    
    currentCarrierAngle += carrierAngleDelta + modulatorSine * modulationAmount;
    
    return carrierSine;
}

void FM2SineOscsGenerator::modulateModulatorMulti (float mod)
{
    modulatorMultiMod = mod >= 0 ? 1 + mod : 1 / (1 - mod * 0.5);
    updateAngleDelta();
}





void FM2SineOscsGenerator::audioProcessorParameterChanged (AudioProcessor* processor,
                                     int parameterIndex,
                                     float newValue)
{
    switch (parameterIndex)
    {
        // gloabl FM amount
        case 0:
            break;
        // modulator frequency multiplier
        case 1:
            updateAngleDelta();
            break;
        default:
            break;
    }
}

void FM2SineOscsGenerator::audioProcessorChanged (AudioProcessor* processor) {}

