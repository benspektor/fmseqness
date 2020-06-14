/*
  ==============================================================================

    PortamentoController.h
    Created: 18 May 2020 3:09:24pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Supporting_Files/Constants.h"

//==============================================================================
/*
*/
class PortamentoController    : public Component
{
public:
    PortamentoController (AudioProcessorValueTreeState& parameters) : mParameters (parameters)
    {
        addAndMakeVisible (portamentoSlider);
        portamentoSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        portamentoSlider.setTextBoxStyle(Slider::NoTextBox, false, 0.0, 0.0);
        
        portamentoAttachment.reset ( new SliderAttachment (mParameters, "portamento", portamentoSlider));
    }

    ~PortamentoController()
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
        g.setColour (Colours::white);
        g.setFont (14.0f);
        Rectangle<float> labelRect { 0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight() - PADDING / 2) };
        g.drawText ("Portamento", labelRect, Justification::centredBottom, true);   // draw some placeholder text
    }

    void resized() override
    {
        portamentoSlider.setBounds ( 0, 0, getHeight(), getHeight() * 4 / 5);
    }

private:
    AudioProcessorValueTreeState& mParameters;
    std::atomic<float>* portamento { mParameters.getRawParameterValue ("portamento") };
    Slider portamentoSlider;
    std::unique_ptr<SliderAttachment> portamentoAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PortamentoController)
};
