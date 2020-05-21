/*
  ==============================================================================

    LFOMeterWindow.h
    Created: 20 May 2020 12:53:07pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LFOMeterBar.h"

//==============================================================================
/*
*/
class LFOMeterWindow    : public Component
{
public:
    LFOMeterWindow()
    {
        // In your constructor, you should add any child components, and
        // initialise any special settings that your component needs.
        addAndMakeVisible (meterBar);
    }

    ~LFOMeterWindow()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..

    }
    
    void drawMeter (float value)
    {
        float innerHeight = getHeight() - 20;
        float innerWidth = getWidth() - 20;
        float barHeight = abs(value) * 0.5 * innerHeight;
        float yPosition = value >= 0.0f ? getHeight() / 2.0f - barHeight : getHeight() / 2.0f;
        
        meterBar.setBounds (10,yPosition, innerWidth, barHeight );
    }

private:
    LFOMeterBar meterBar;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LFOMeterWindow)
};
