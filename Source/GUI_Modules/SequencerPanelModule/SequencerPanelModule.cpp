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
    
    
    playStopAttachment.reset ( new ButtonAttachment (mParameters, "play", playStopButton));
}

SequencerPanelModule::~SequencerPanelModule()
{
}

void SequencerPanelModule::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("SequencerPanelModule", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void SequencerPanelModule::resized()
{
    float height = getHeight();
    float width  = getWidth();
    
    float playStopX            = width - height;
    float swingControllerWidth = height * 2;
    
    playStopButton .setBounds ( playStopX, 0, height, height );
    swingController.setBounds ( 1, 1, swingControllerWidth, height - 2 );
    tempoController.setBounds ( swingControllerWidth + PADDING, 1, height, height - 2);
    
}



