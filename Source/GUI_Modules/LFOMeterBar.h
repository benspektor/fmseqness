/*
  ==============================================================================

    LFOMeterBar.h
    Created: 20 May 2020 12:43:40pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LFOMeterBar    : public Component
{
public:
    LFOMeterBar()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.

    }

    ~LFOMeterBar()
    {
    }

    void paint (Graphics& g) override
    {
        g.setGradientFill (ColourGradient (Colours::darkgrey, 0,  0, Colours::black,  getWidth(),  0, false));
        g.fillAll();
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOMeterBar)
};
