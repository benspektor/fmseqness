/*
  ==============================================================================

    PitchLineScreen.h
    Created: 28 Apr 2020 9:40:53pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../../Supporting_Files/SupportingFiles.h"

//==============================================================================
/*
*/
class PitchLineScreen    : public Component
{
public:
    PitchLineScreen (AudioProcessorValueTreeState& parameters, Rectangle<float> (&recs)[TOTAL_NUMBER_OF_STEPS]) : mParameters (parameters), mRecs(recs)
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
            if (*mParameters.getRawParameterValue(STEPS_GATE[stepNumber]) == GATE_GLIDE)
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
    AudioProcessorValueTreeState& mParameters;
//    PitchControllerValuesModel& mDataModel;
//    StepGateStateValuesModel& mGateModel;
    Rectangle<float> (&mRecs)[TOTAL_NUMBER_OF_STEPS];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchLineScreen)
};
