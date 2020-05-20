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
//        tempoKnob.setIncDecButtonsMode (Slider::IncDecButtonMode::incDecButtonsDraggable_Vertical);
        tempoAttachment.reset (new SliderAttachment (mParameters, "tempo", tempoKnob));
    }

    ~TempoController()
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

//        g.setColour (Colours::grey);
//        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("Tempo", Rectangle<float>(0, 0, getWidth(), getHeight() - PADDING / 2),
                    Justification::centredBottom, true);   // draw some placeholder text
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
