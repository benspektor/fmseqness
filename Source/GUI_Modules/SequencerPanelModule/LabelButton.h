/*
  ==============================================================================

    LabelButton.h
    Created: 11 May 2020 9:46:46pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class LabelButton    : public Component
{
public:
    LabelButton(const String& buttonName) : name (buttonName)
    {
        
    }

    ~LabelButton()
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
        g.setColour (colour);
        g.setFont (14.0f);
        g.drawText (name, getLocalBounds(),
                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
    }
    
    void mouseEnter (const MouseEvent& event) override
    {
        colour = Colours::lightblue;
        repaint();
    }
    
    void mouseExit (const MouseEvent& event) override
    {
        colour = Colours::white;
        repaint();
    }
    
    
    void mouseDown (const MouseEvent& event) override
    {
        onClick();
        colour = Colours::black;
        repaint();
    }

    void mouseUp (const MouseEvent& event) override
    {
        colour = Colours::white;
        repaint();
    }
    
    std::function<void()> onClick;

private:
    
    Colour colour = Colours::white;
    String name;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LabelButton)
};
