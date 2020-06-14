/*
  ==============================================================================

    TempoController.h
    Created: 12 May 2020 10:15:54am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class TempoController    : public Component
{
public:
    TempoController(AudioProcessorValueTreeState& parameters) : mParameters (parameters)
    {
        addAndMakeVisible (tempoKnob);
        tempoKnob.setSliderStyle(Slider::SliderStyle::IncDecButtons);
        tempoKnob.setTextBoxStyle(Slider::TextBoxAbove, false, 50.0, 20.0);
        tempoAttachment.reset (new SliderAttachment (mParameters, "tempo", tempoKnob));
    }

    ~TempoController()
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
        g.setColour (Colours::white);
        g.setFont (14.0f);
        Rectangle<float> labelRect { 0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight() - PADDING / 2) };
        g.drawText ("Tempo", labelRect, Justification::centredBottom, true);   // draw some placeholder text
    }

    void resized() override
    {
        tempoKnob.setBounds (PADDING,PADDING, getWidth() - PADDING * 2, getHeight() - PADDING * 3);
    }

private:
    AudioProcessorValueTreeState& mParameters;
    std::atomic<float>* swingValue { mParameters.getRawParameterValue ("tempo") };
    Slider tempoKnob;
    std::unique_ptr<SliderAttachment> tempoAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TempoController)
};
