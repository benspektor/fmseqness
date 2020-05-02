/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FmseqnessAudioProcessorEditor::FmseqnessAudioProcessorEditor (FmseqnessAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    stepSeqModule.reset ( new StepperSequencerModule ( processor.getStepperDataModel() ) );
    addAndMakeVisible(*stepSeqModule);
    setSize (600, 400);
    setResizable(true, true);
}

FmseqnessAudioProcessorEditor::~FmseqnessAudioProcessorEditor()
{
}

//==============================================================================
void FmseqnessAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

   
}

void FmseqnessAudioProcessorEditor::resized()
{
    stepSeqModule->setBounds ( getLocalBounds() );
}
