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
    stepSeqModule  .reset ( new StepperSequencerModule ( processor.getStepperDataModel(), processor.getParametersTree()));
    sinesGUI       .reset ( new FMSinesGUI (processor.getParametersTree()));
    seqGUI         .reset ( new SequencerControlGUI (processor.getParametersTree()));
    ampAhdEnvWindow.reset ( new AHDEnvWindow (processor.getAmpAHDEnvDataModel(), "Amp"));
    modAhdEnvWindow.reset ( new AHDEnvWindow (processor.getModAHDEnvDataModel(), "Mod"));
    
    addAndMakeVisible (*stepSeqModule);
    addAndMakeVisible (*sinesGUI);
    addAndMakeVisible (*seqGUI);
    addAndMakeVisible (*ampAhdEnvWindow);
    addAndMakeVisible (*modAhdEnvWindow);
    
    
    
    setResizable (true, true);
    setSize (1200, 800);
    processor.addListener (this);
    processor.addListener (stepSeqModule.get());
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
    sinesGUI->setBounds(10, 10, 230, 160);
    seqGUI->setBounds(250, 10, 300, 160);
    ampAhdEnvWindow->setBounds(560, 10, 300, 160);
    modAhdEnvWindow->setBounds(870, 10, 300, 160);
    stepSeqModule->setBounds ( 10, 180, getWidth() - 20, getHeight() - 180 );
    
//    stepSeqModule->drawGreyedOut();
}

void FmseqnessAudioProcessorEditor::timerCallback()
{
    stepSeqModule->timerTic();
}

void FmseqnessAudioProcessorEditor::audioProcessorParameterChanged (AudioProcessor* processor,
                                                int parameterIndex,
                                                float newValue)
{

    if (parameterIndex == 4)
    {
        if (newValue == 1.0f)
        {
            startTimerHz(60);
            stepSeqModule->play();
        }
            
        else
        {
            stopTimer();
            stepSeqModule->stop();
        }
            
    }
}

 
void FmseqnessAudioProcessorEditor::audioProcessorChanged (AudioProcessor* processor) {}
