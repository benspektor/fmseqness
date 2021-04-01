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

enum LFOShape {saw, sine, triangle, square};

class LFO : public AudioProcessorListener
{
public:
    void extracted();
    
LFO (AudioProcessorValueTreeState& parameters);
    ~LFO();
    
    void updateAngleDelta();
    void setSampleRate (double sampleRate);
    void restart();
    void updatePolarity();
    float generate (LFOShape shape);
    
    //AudioProcessorListener
    void audioProcessorParameterChanged (AudioProcessor* processor,
                                         int parameterIndex,
                                         float newValue) override;
    void audioProcessorChanged (AudioProcessor* processor, const ChangeDetails& details) override;
    
private:
    AudioProcessorValueTreeState& mParameters;
    std::atomic<float>* frequency { mParameters.getRawParameterValue ("lfoFrequency") };
    std::atomic<float>* length    { mParameters.getRawParameterValue ("LfoLength") };
    std::atomic<float>* tempo     { mParameters.getRawParameterValue ("tempo") };
    std::atomic<float>* phase     { mParameters.getRawParameterValue ("lfoPhase") };
    std::atomic<float>* polarity  { mParameters.getRawParameterValue ("lfoPolarity") };
    std::atomic<float>* stepSync  { mParameters.getRawParameterValue ("lfoStepSync") };
    float amp = 0.0, delta = 0.0;
    double currentSampleRate = 0.0;
    bool  isUni = false;
    float direction = 1.0f;
};
