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
    Value attack;
    Value hold;
    Value decay;
    Value level;
    Value attackCurve;
    Value decayCurve;
};
