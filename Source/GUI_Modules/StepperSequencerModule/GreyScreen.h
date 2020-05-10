/*
  ==============================================================================

    GreyScreen.h
    Created: 10 May 2020 6:41:15pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class GreyScreen    : public Component
{
public:
    GreyScreen()
    {
    }

    ~GreyScreen()
    {
    }

    void paint (Graphics& g) override
    {
        g.setColour (Colours::grey);
        g.fillAll();
    }

    void resized() override
    {
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GreyScreen)
};
