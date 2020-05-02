/*
  ==============================================================================

    PitchLineScreen.h
    Created: 28 Apr 2020 9:40:53pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../StepperSequencerConstants.h"

//==============================================================================
/*
*/
class PitchLineScreen    : public Component
{
public:
    PitchLineScreen (PitchControllerValuesModel& dataModel, StepGateStateValuesModel& gateModel, Rectangle<float> (&recs)[TOTAL_NUMBER_OF_STEPS]) : mDataModel(dataModel), mGateModel(gateModel), mRecs(recs)
    {
    }

    ~PitchLineScreen()
    {
    }

    void paint (Graphics& g) override
    {
        g.setColour(Colours::black);
        
        for (auto stepNumber = 0; stepNumber < TOTAL_NUMBER_OF_STEPS; ++stepNumber)
        {
            if (mGateModel.values[stepNumber] == glide)
            {
                Path path;
                path.startNewSubPath ( mRecs[stepNumber].getCentre() );
                path.lineTo ( mRecs[stepNumber - 1].getCentre() );
                g.strokePath (path, PathStrokeType(PITCH_LINE_THIKNESS));
            }
        }
    }

    void resized() override
    {
    }

private:
    PitchControllerValuesModel& mDataModel;
    StepGateStateValuesModel& mGateModel;
    Rectangle<float> (&mRecs)[TOTAL_NUMBER_OF_STEPS];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchLineScreen)
};
