/*
  ==============================================================================

    PlayStopButton.h
    Created: 11 May 2020 10:08:05am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PlayStopButton :  public ToggleButton
{
public:
    PlayStopButton()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        KeyPress spaceBarPress (32);
        addShortcut(spaceBarPress);
        setTooltip ("<SpaceBar>");
    }

    ~PlayStopButton()
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText (label, getLocalBounds(), Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
    }
    
    void clicked() override
    {
        label = getToggleState() ? "Stop" : "Play";
    }

private:
    String label = "Play";
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlayStopButton)
};
