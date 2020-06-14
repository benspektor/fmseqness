/*
  ==============================================================================

    SwingController.h
    Created: 11 May 2020 8:24:17pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Supporting_Files/Constants.h"
#include "LabelButton.h"

//==============================================================================
/*
*/
class SwingController    : public Component
{
public:
    SwingController (AudioProcessorValueTreeState& parameters) : mParameters (parameters)
    {
        addAndMakeVisible (noneButton);
        addAndMakeVisible (tripletButton);
        addAndMakeVisible (dottedButton);
        addAndMakeVisible (insaneButton);
        
        noneButton   .onClick = [this]() { swingSlider.setValue (SWING_MIN_VALUE); };
        tripletButton.onClick = [this]() { swingSlider.setValue (SWING_TRIPLET_VALUE); };
        dottedButton .onClick = [this]() { swingSlider.setValue (SWING_DOT_VALUE); };
        insaneButton .onClick = [this]() { swingSlider.setValue (SWING_MAX_VALUE); };
               
        addAndMakeVisible (swingSlider);
        swingSlider.setTextBoxStyle (juce::Slider::TextEntryBoxPosition::NoTextBox, true, 0, 0);
        swingAttachment.reset ( new SliderAttachment (mParameters, "swingValue", swingSlider));
    }

    ~SwingController()
    {
    }

    void paint (Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
        g.setColour (Colours::white);
        g.setFont (14.0f);
        Rectangle<float> labelRect { 0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight() - PADDING / 2) };
        g.drawText ("Swing!", labelRect, Justification::centredBottom, true);   // draw some placeholder text
    }

    void resized() override
    {
        float buttonWidth       = (getWidth() - PADDING * 5)  / 4.0f;
        float buttonHeight      = (getHeight() - PADDING * 2) / 3.0f;
        float swingSliderHeight = PADDING * 1.5 + buttonHeight;
        
        noneButton   .setBounds (PADDING, PADDING, buttonWidth, buttonHeight);
        tripletButton.setBounds (PADDING * 2 + buttonWidth, PADDING, buttonWidth, buttonHeight);
        dottedButton .setBounds (PADDING * 3 + buttonWidth * 2, PADDING, buttonWidth, buttonHeight);
        insaneButton .setBounds (PADDING * 4 + buttonWidth * 3, PADDING, buttonWidth, buttonHeight);
        swingSlider  .setBounds (PADDING, swingSliderHeight, getWidth() - PADDING * 2, buttonHeight);
    }

private:
    AudioProcessorValueTreeState& mParameters;
    LabelButton noneButton {"Non"}, tripletButton {"Tri"}, dottedButton {"Dot"}, insaneButton {"Mad"};
    std::atomic<float>* swingValue { mParameters.getRawParameterValue ("swingValue") };
    Slider swingSlider;
    std::unique_ptr<SliderAttachment> swingAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SwingController)
};
