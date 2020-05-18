/*
  ==============================================================================

    SequencerPanelModule.cpp
    Created: 11 May 2020 9:34:48am
    Author:  Ben Spector

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SequencerPanelModule.h"

//==============================================================================
SequencerPanelModule::SequencerPanelModule (AudioProcessorValueTreeState& parameters) : mParameters(parameters)
{
    addAndMakeVisible (swingController);
    addAndMakeVisible (playStopButton);
    addAndMakeVisible (tempoController);
    addAndMakeVisible (basePitchPicker);
    
    addAndMakeVisible (portamentoSlider);
    portamentoSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    portamentoSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50.0, 20.0);
    
    portamentoAttachment.reset ( new SliderAttachment (mParameters, "portamento", portamentoSlider));
    
    playStopAttachment.reset ( new ButtonAttachment (mParameters, "play", playStopButton));
}

SequencerPanelModule::~SequencerPanelModule()
{
}

void SequencerPanelModule::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

}

void SequencerPanelModule::resized()
{
    float height = getHeight();
    float width  = getWidth();
    
    float playStopX            = width - height;
    float swingControllerWidth = height * 2;
    float tempoControllerX     = swingControllerWidth + PADDING;
    float basePitchPickerX     = swingControllerWidth + height + PADDING * 2;
    float portamentoSliderX    = basePitchPickerX + height + PADDING;
    
    playStopButton .setBounds ( playStopX, 0, height, height );
    swingController.setBounds ( 1, 1, swingControllerWidth, height - 2 );
    tempoController.setBounds ( tempoControllerX, 1, height, height - 2 );
    basePitchPicker.setBounds ( basePitchPickerX, 1, height, height - 2 );
    
    portamentoSlider.setBounds ( portamentoSliderX, 1, height, height);
}



