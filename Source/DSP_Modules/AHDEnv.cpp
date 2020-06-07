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
    mModel.attack      .addListener ( this );
    mModel.hold        .addListener ( this );
    mModel.decay       .addListener ( this );
    mModel.attackCurve .addListener ( this );
    mModel.decayCurve  .addListener ( this );
    mModel.level       .addListener ( this );
    
    mModel.addActionListener(this);
}

AHDEnv::~AHDEnv() {}

double AHDEnv::process ()
{
    if (state == PlayState::stop)
        return 0.0;

    //Attack stage:
    attack = attack < 1.0 ? attack + attackDelta : 1.0;
    
    //Hold stage:
    if ( attack == 1.0 && hold > 0.0)
        hold = hold > 0.0 ? hold - holdDelta : 0.0;
    
    //Decay stage:
    else if (attack == 1.0 && hold <= 0.0)
        decay = decay > 0.0 ? decay - decayDelta : 0.0;
    
    auto amp = (float)mModel.level.getValue() *  pow (attack, attackCurve) * pow (decay, decayCurve);
    
    return amp;
}

void AHDEnv::reset (float currentSample, float length)
{
    currentStepLength = length;
    
    attack = 0.0;
    decay  = 1.0;
    hold   = 1.0;
    
    updateValues();
}

void AHDEnv::startDecay()
{
    hold = 0.0;
}

void AHDEnv::setSampleRate (double sampleRate)
{
    this->sampleRate = sampleRate;
    updateValues(); 
}

void AHDEnv::updateValues()
{
    stepLength   = ((60 / tempo->load()) / 4) * currentStepLength;
    restartDelta = FMUtilities::convertTimeToFrequency (RESTART_TIME , sampleRate);
    attackDelta  = FMUtilities::convertTimeToFrequency (((float)mModel.attack.getValue() + 0.01f) * stepLength , sampleRate);
    holdDelta    = FMUtilities::convertTimeToFrequency ((float)mModel.hold.getValue() * stepLength , sampleRate);
    decayDelta   = FMUtilities::convertTimeToFrequency (((float)mModel.decay.getValue() + 0.01) * stepLength , sampleRate);
    attackCurve  = FMUtilities::convert0to1toCurveValue ((float)mModel.attackCurve.getValue());
    decayCurve   = FMUtilities::convert0to1toCurveValue ((float)mModel.decayCurve.getValue());
}

void AHDEnv::valueChanged (Value& value)
{
    updateValues();
}

void AHDEnv::actionListenerCallback (const String& message)
{
    updateValues();
}
