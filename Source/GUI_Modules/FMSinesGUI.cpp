/*
  ==============================================================================

    FMSinesGUI.cpp
    Created: 9 Feb 2020 1:26:11pm
    Author:  Ben Spector

  ==============================================================================
*/

#include <JuceHeader.h>
#include "FMSinesGUI.h"

//==============================================================================
FMSinesGUI::FMSinesGUI (AudioProcessorValueTreeState& parameters) : mParameters(parameters)
{
    addAndMakeVisible(fmAmountLabel);
    fmAmountLabel.setText("FM amount", NotificationType::dontSendNotification);
    fmAmountLabel.setFont (Font (16.0f, Font::plain));
    fmAmountLabel.setJustificationType(Justification::centred);

    addAndMakeVisible(modMultiLabel);
    modMultiLabel.setText("Modulator Multi", NotificationType::dontSendNotification);
    modMultiLabel.setFont (Font (16.0f, Font::plain));
    modMultiLabel.setJustificationType(Justification::centred);
    
    addAndMakeVisible (fmAmountKnob);
    fmAmountKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    fmAmountKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50.0, 20.0);
    
    addAndMakeVisible (modMultiKnob);
    modMultiKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    modMultiKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50.0, 20.0);
    
    fmAmountAttachment.reset ( new SliderAttachment (mParameters, "globalFMAmount", fmAmountKnob));
    modMultiAttachment.reset ( new SliderAttachment (mParameters, "modulatorMultiplier", modMultiKnob));
}

FMSinesGUI::~FMSinesGUI() {}

void FMSinesGUI::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("FM Sines", getLocalBounds(),
                Justification::centredTop, true);   // draw some placeholder text

   
}

void FMSinesGUI::resized()
{
    fmAmountKnob.setBounds (10, 50, 100, 100);
    modMultiKnob.setBounds (120, 50, 100, 100);
    
    fmAmountLabel.setBounds(10, 20, 100, 20);
    modMultiLabel.setBounds(120, 20, 100, 20);

}


