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
//    void updateAngleDelta();
//    void setModulatorMultiFactor (double factor);
    void setSampleRate (double sampleRate);
//    void setCurrentPitch (double pitch);
//    void setStepFMModMulti (double factor);
//    void modulateModulatorMulti (float mod);
//    float generate(float externalModulationAmount);
    float generate(float pitch, float fmAmount, float modMulti);
    
//    //AudioProcessorListener
//    void audioProcessorParameterChanged (AudioProcessor* processor,
//                                         int parameterIndex,
//                                         float newValue) override;
//    void audioProcessorChanged (AudioProcessor* processor) override;
    
private:
//    AudioProcessorValueTreeState& mParameters;
//    
//    std::atomic<float>* fmAmount       { mParameters.getRawParameterValue ("globalFMAmount") };
//    std::atomic<float>* modulatorMulti { mParameters.getRawParameterValue ("modulatorMultiplier") };
    
    float currentCarrierAngle = 0.0;
    float currentModulatorAngle = 0.0;
    double currentSampleRate = 0.0;
//    double normalizedCurrentCarrierFrequency = 0.0;
//    float modulatorMultiMod = 0.0f;
};
