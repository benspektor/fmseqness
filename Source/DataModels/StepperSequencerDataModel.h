/*
  ==============================================================================

    StepperSequencerDataModel.h
    Created: 25 Apr 2020 11:42:15pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


//enum StepGateState { on, glide, off };

struct BarsControllerValuesModel
{
    float values[32] = {0.0f};
};

struct PitchControllerValuesModel
{
//    std::atomic<float>* values[32];
//    Value values[32];
    std::unique_ptr<Value> values[32];
//    std::vector<std::unique_ptr<Value>> values;
};

//struct StepGateStateValuesModel
//{
//    StepGateState values[32] = { StepGateState::on };
//};


//struct StepperSequencerDataModel
//{
//    PitchControllerValuesModel pitchValues;
//    BarsControllerValuesModel  fmValues;
//    BarsControllerValuesModel  modMultiValues;
//    BarsControllerValuesModel  seqModValues;
////    StepGateStateValuesModel   gateStateValues;
//
////    int getStepLength (int stepIndex)
////    {
////        int endIndex = stepIndex + 1;
////
////        while (gateStateValues.values[endIndex] == glide)
////            endIndex++;
////
////        return endIndex - stepIndex;
////    }
////
////    bool isNextStepGlide (int stepIndex)
////    {
////        return gateStateValues.values[stepIndex + 1] == glide;
////    }
//};
