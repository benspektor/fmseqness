/*
  ==============================================================================

    AHDEnvDataModel.h
    Created: 26 Feb 2020 10:37:29am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>


struct AHDEnvDataModel
{
//    std::atomic<float>* attack;
//    std::atomic<float>* hold;
//    std::atomic<float>* decay;
//    std::atomic<float>* level;
//    std::atomic<float>* attackCurve;
//    std::atomic<float>* decayCurve;
    
    Value attack;
    Value hold;
    Value decay;
    Value level;
    Value attackCurve;
    Value decayCurve;
    
//    float getAttack()
//    {
//        return float(attack);
//    }
};
