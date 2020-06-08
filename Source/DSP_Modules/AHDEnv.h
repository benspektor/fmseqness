/*
  ==============================================================================

    AHDEnv.h
    Created: 6 Feb 2020 10:11:28am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include "../Supporting_Files/Enums.h"
#include "../Supporting_Files/FMUtilities.h"
#include "../Supporting_Files/Constants.h"
#include "../DataModels/AHDEnvDataModel.h"

class AHDEnv : Value::Listener, ActionListener
{
    
public:
    AHDEnv(AudioProcessorValueTreeState& parameters, AHDEnvDataModel& model);
    ~AHDEnv();
    
    float process();
    void setSampleRate (double sampleRate);
    void reset (float currentSample, float length);
    void startDecay();
    void updateValues();
    void valueChanged (Value& value) override;
    void actionListenerCallback (const String& message) override;
    PlayState state = PlayState::stop;
    
private:
    AudioProcessorValueTreeState& mParameters;
    AHDEnvDataModel& mModel;
    std::atomic<float>* tempo {mParameters.getRawParameterValue("tempo")};
    
    double sampleRate;
    float restart = 0.0 , attack = 0.0, hold = 0.0, decay = 0.0;
    float currentStepLength = 1.0f;
    
    float stepLength = 1.0f;
    float restartDelta = 1.0f;
    float attackDelta = 1.0f;
    float holdDelta = 1.0f;
    float decayDelta = 1.0f;
    float attackCurve = 1.0f;
    float decayCurve = 1.0f;
};
