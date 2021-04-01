/*
  ==============================================================================

    Bar.h
    Created: 10 Apr 2020 1:10:04am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class Bar    : public Component
{
public:
    Bar(bool isDownBeat)
    {
        this->isDownBeat = isDownBeat;
    }

    ~Bar() {}

    void paint (Graphics& g) override
    {
        if (isCurrentStep)
            g.setGradientFill (ColourGradient (currentStepColor, 0,  0, endColor,   getWidth(),  0, false));
        
        else if (isDownBeat)
            g.setGradientFill (ColourGradient (downBeatColor, 0,  0, endColor,  getWidth(),  0, false));
        else
            g.setGradientFill (ColourGradient (color, 0,  0, endColor,  getWidth(),  0, false));

        g.fillAll();
    }

    void resized() override {}
    
    void lightUp()
    {
        isCurrentStep = true;
        repaint();
    }
    
    void turnOff()
    {
        isCurrentStep = false;
        repaint();
    }

private:
    bool isDownBeat = false;
    bool isCurrentStep = false;
    Colour downBeatColor = Colours::grey;
    Colour color = Colours::darkgrey;
    Colour currentStepColor = Colours::whitesmoke;
    uint8 cN = 100;
    Colour endColor {cN,cN,cN};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Bar)
};
