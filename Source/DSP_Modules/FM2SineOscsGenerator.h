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


class FM2SineOscsGenerator : public AudioProcessorListener
{
public:
    FM2SineOscsGenerator (AudioProcessorValueTreeState& parameters);
    ~FM2SineOscsGenerator();
    void updateAngleDelta();
    void setFMAmountValue (double amount);
    void setModulatorMultiFactor (double factor);
    void setSampleRate (double sampleRate);
    void setCurrentPitch (double pitch);
    void setStepFMModMulti (double factor);
    float generate(float externalModulationAmount);
    
    //AudioProcessorListener
    void audioProcessorParameterChanged (AudioProcessor* processor,
                                         int parameterIndex,
                                         float newValue) override;
    void audioProcessorChanged (AudioProcessor* processor) override;
    
private:
    AudioProcessorValueTreeState& mParameters;
    
    std::atomic<float>* fmAmount { mParameters.getRawParameterValue ("globalFMAmount") };
    std::atomic<float>* modulatorMulti { mParameters.getRawParameterValue ("modulatorMultiplier") };
    
    double currentCarrierAngle = 0.0, carrierAngleDelta = 0.0;
    double currentModulatorAngle = 0.0, modulatorAngleDelta = 0.0, stepFMModMulti = 1.0;
    double currentSampleRate = 0.0;
    double normalizedCurrentCarrierFrequency = 0.0;
};
