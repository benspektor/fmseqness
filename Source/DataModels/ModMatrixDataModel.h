/*
  ==============================================================================

    ModMatrixDataModel.h
    Created: 4 Jun 2020 2:22:08pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

struct ModulationInstancModel
{
    std::atomic<float>* amount;
    std::atomic<float>* source;
    std::atomic<float>* destination;
};

struct ModMatrixModel
{
    std::vector<ModulationInstancModel> mods;
};
