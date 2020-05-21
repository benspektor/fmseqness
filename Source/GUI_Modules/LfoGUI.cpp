/*
  ==============================================================================

    LfoGUI.cpp
    Created: 20 May 2020 10:26:01am
    Author:  Ben Spector

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LfoGUI.h"

//==============================================================================
LfoGUI::LfoGUI (AudioProcessorValueTreeState& parameters) : mParameters(parameters)
{
    addAndMakeVisible(meterWindow);
    addAndMakeVisible(lengthSlider);
    lengthSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    lengthSlider.setTextBoxStyle(Slider::TextBoxAbove, false, 50.0, 20.0);
    lengthAttachment.reset ( new SliderAttachment (mParameters, "LfoLength", lengthSlider ));

}

LfoGUI::~LfoGUI()
{
}

void LfoGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::grey);
    g.setFont (30.0f);
    g.drawText ("LFO", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void LfoGUI::resized()
{
    float meterWindowWidth = getWidth() / 5.0f;
    float meterWindowX = getWidth() - PADDING - meterWindowWidth;
    
    lengthSlider.setBounds(PADDING, PADDING, 50, 50);
    meterWindow.setBounds(meterWindowX, PADDING, meterWindowWidth, getHeight() - PADDING * 2);
    
}

void LfoGUI::timerTic(float value)
{
    meterWindow.drawMeter(value);
}
