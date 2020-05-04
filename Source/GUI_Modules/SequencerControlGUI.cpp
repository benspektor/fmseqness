/*
  ==============================================================================

    SequencerControlGUI.cpp
    Created: 9 Feb 2020 9:49:16pm
    Author:  Ben Spector

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SequencerControlGUI.h"

//==============================================================================
SequencerControlGUI::SequencerControlGUI(AudioProcessorValueTreeState& parameters) : mParameters(parameters)
{
   addAndMakeVisible(tempoLabel);
   tempoLabel.setText("Tempo", NotificationType::dontSendNotification);
   tempoLabel.setFont (Font (16.0f, Font::plain));
   tempoLabel.setJustificationType(Justification::centred);
   
   addAndMakeVisible(numOfStepsLabel);
   numOfStepsLabel.setText("Steps", NotificationType::dontSendNotification);
   numOfStepsLabel.setFont (Font (16.0f, Font::plain));
   numOfStepsLabel.setJustificationType(Justification::centred);
    
    addAndMakeVisible (tempoKnob);
    tempoKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    tempoKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50.0, 20.0);

    addAndMakeVisible (numOfStepsKnob);
    numOfStepsKnob.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    numOfStepsKnob.setTextBoxStyle(Slider::TextBoxBelow, false, 50.0, 20.0);

    tempoAttachment.reset (new SliderAttachment (mParameters, "tempo", tempoKnob));
    stepsAttachment.reset (new SliderAttachment (mParameters, "steps", numOfStepsKnob));

    addAndMakeVisible(playButton);
    playButton.setButtonText("Play");
    playButton.setColour(TextButton::buttonColourId, Colours::black);
    playAttachment.reset( new ButtonAttachment(mParameters, "play", playButton));
}

SequencerControlGUI::~SequencerControlGUI() {}

void SequencerControlGUI::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("Sequencer Control", getLocalBounds(),
                Justification::centredTop, true);   // draw some placeholder text
}

void SequencerControlGUI::resized()
{
    tempoKnob.setBounds (10, 50, 100, 100);
    numOfStepsKnob.setBounds (120, 50, 100, 100);
    tempoLabel.setBounds(10, 20, 100, 20);
    numOfStepsLabel.setBounds(120, 20, 100, 20);
    playButton.setBounds(230, 60, 50, 50);
}

