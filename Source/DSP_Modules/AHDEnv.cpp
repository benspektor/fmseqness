/*
  ==============================================================================

    AHDEnv.cpp
    Created: 6 Feb 2020 10:11:28am
    Author:  Ben Spector

  ==============================================================================
*/

#include "AHDEnv.h"

AHDEnv::AHDEnv (AudioProcessorValueTreeState& parameters, AHDEnvDataModel& model) : mParameters(parameters), mModel(model)
{
}

AHDEnv::~AHDEnv() {}

double AHDEnv::process(double currentSampleRate)
{
    if (state == PlayState::stop)
        return 0.0;

    float stepLength = 60 / tempo->load() / 4 * currentStepLength;
    
    auto restartDelta = FMUtilities::convertTimeToFrequency(RESTART_TIME , currentSampleRate);
    auto attackDelta  = FMUtilities::convertTimeToFrequency((mModel.attack.load() + 0.01) * stepLength , currentSampleRate);
    auto holdDelta    = FMUtilities::convertTimeToFrequency(mModel.hold.load() * stepLength , currentSampleRate);
    auto decayDelta   = FMUtilities::convertTimeToFrequency((mModel.decay.load() + 0.01) * stepLength , currentSampleRate);
    
    //restart stage;
    restart = restart > 0.0 ? restart - restartDelta : 0.0;
    
    //Attack stage:
    if (restart == 0.0)
        attack = attack < 1.0 ? attack + attackDelta : 1.0;
    
    //Hold stage:
    if (isNextStepGlide == false && attack == 1.0 && hold > 0.0)
        hold = hold > 0.0 ? hold - holdDelta : 0.0;
    
    //Decay stage:
    if (attack == 1.0 && hold <= 0.0)
        decay = decay > 0.0 ? decay - decayDelta : 0.0;
    
    auto amp = mModel.level.load() * (restart + pow (attack, mModel.attackCurve) * pow (decay, mModel.decayCurve));
    
    //Envelope end:
    if (decay == 0.0)
    {
        state = PlayState::stop;
        reset(0.0, currentSampleRate, false, 1);
    }
    
    return amp;
}

void AHDEnv::reset (double currentSample, double currentSampleRate, bool isNextStepGlide, int length)
{
    this->isNextStepGlide = isNextStepGlide;
    currentStepLength = length;
    
    if (retrigger)
    {
        restart = currentSample;
        attack = 0.0;
    }
    else
    {
        restart = 0.0;
        attack = currentSample;
    }
    
    decay = 1.0;
    hold = 1.0;
}

void AHDEnv::startDecay()
{
    isNextStepGlide = false;
    hold = 0.0;
}
