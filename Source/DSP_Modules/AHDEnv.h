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

class AHDEnv
{
    
public:
    AHDEnv(AudioProcessorValueTreeState& parameters, AHDEnvDataModel& model);
    ~AHDEnv();
    PlayState state = PlayState::stop;
    double process (double currentSampleRate);
    void reset (double currentSample, double currentSampleRate, bool isNextStepGlide, float length);
    void startDecay();
    
private:
    AudioProcessorValueTreeState& mParameters;
    AHDEnvDataModel& mModel;
    
//    std::atomic<float>* attackTime {mParameters.getRawParameterValue("attack")};
//    std::atomic<float>* holdTime {mParameters.getRawParameterValue("hold")};
//    std::atomic<float>* decayTime {mParameters.getRawParameterValue("decay")};
    std::atomic<float>* tempo {mParameters.getRawParameterValue("tempo")};
    
    float restart = 0.0 ,attack = 0.0, hold = 0.0, decay = 0.0;
    bool retrigger = true;
    float currentStepLength = 1.0f;
    bool isNextStepGlide = false;
};
