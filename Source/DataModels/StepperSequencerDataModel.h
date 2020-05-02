/*
  ==============================================================================

    StepperSequencerDataModel.h
    Created: 25 Apr 2020 11:42:15pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

enum StepGateState { on, glide, off };

struct BarsControllerValuesModel
{
    float values[32] = {1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0,1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
};

struct PitchControllerValuesModel
{
    int values[32] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
};

struct StepGateStateValuesModel
{
    StepGateState values[32] = {StepGateState::on, StepGateState::glide, StepGateState::glide, StepGateState::on,
                                StepGateState::on, StepGateState::glide, StepGateState::off, StepGateState::on,
                                StepGateState::on, StepGateState::glide, StepGateState::glide, StepGateState::on,
                                StepGateState::on, StepGateState::glide, StepGateState::off, StepGateState::on,
                                StepGateState::on, StepGateState::glide, StepGateState::glide, StepGateState::on,
                                StepGateState::on, StepGateState::glide, StepGateState::off, StepGateState::on,
                                StepGateState::on, StepGateState::glide, StepGateState::glide, StepGateState::on,
                                StepGateState::on, StepGateState::glide, StepGateState::off, StepGateState::on
    };
};


struct StepperSequencerDataModel
{
    PitchControllerValuesModel pitchValues;
    BarsControllerValuesModel  levelValues;
    BarsControllerValuesModel  modValues;
    StepGateStateValuesModel   gateStateValues;
};
