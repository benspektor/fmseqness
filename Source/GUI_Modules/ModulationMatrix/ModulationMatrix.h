/*
  ==============================================================================

    ModulationMatrix.h
    Created: 26 May 2020 10:54:57pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "ModulationModule.h"

//==============================================================================
/*
*/
class ModulationMatrix    : public Component
{
public:
    ModulationMatrix(AudioProcessorValueTreeState& parameters) : mParameters (parameters)
    {
        addAndMakeVisible(mod1);
        addAndMakeVisible(mod2);
        addAndMakeVisible(mod3);
        addAndMakeVisible(mod4);
    }

    ~ModulationMatrix()
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

//        g.setColour (Colours::white);
//        g.setFont (14.0f);
//        g.drawText ("ModulationMatrix", getLocalBounds(),
//                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        float modWidth = getWidth() / 4.0f;
        mod1.setBounds(0, 0, modWidth, getHeight());
        mod2.setBounds(modWidth, 0, modWidth, getHeight());
        mod3.setBounds(modWidth * 2, 0, modWidth, getHeight());
        mod4.setBounds(modWidth * 3, 0, modWidth, getHeight());
    }

private:
    AudioProcessorValueTreeState& mParameters;
    ModulationModule mod1 {1, mParameters} , mod2 {2, mParameters}, mod3 {3, mParameters}, mod4 {4, mParameters};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationMatrix)
};
