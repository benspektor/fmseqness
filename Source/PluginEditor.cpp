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
    ampAhdEnvWindow.reset ( new AHDEnvWindow (processor.getAmpAHDEnvDataModel(), "Amp"));
    modAhdEnvWindow.reset ( new AHDEnvWindow (processor.getModAHDEnvDataModel(), "Mod"));
    seqPanel       .reset ( new SequencerPanelModule( processor.getParametersTree() ));
    lfoGUI         .reset ( new LfoGUI ( processor.getParametersTree() ));
    
    addAndMakeVisible ( *stepSeqModule );
    addAndMakeVisible ( *sinesGUI );
    addAndMakeVisible ( *ampAhdEnvWindow );
    addAndMakeVisible ( *modAhdEnvWindow );
    addAndMakeVisible ( *seqPanel );
    addAndMakeVisible ( *lfoGUI );
    
    setResizable (true, true);
    setSize (1200, 900);
    
    processor.addListener (this);
    processor.addListener (stepSeqModule.get());
    stepSeqModule->addActionListener(this);
    
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
    float innerWidth          = getWidth() - PADDING * 2;
    float stepSeqModuleY      = PADDING * 2 + ENVELOPE_WINDOW_HEIGHT;
    float stepSeqModuleHeight = getHeight() - PADDING * 3 - ENVELOPE_WINDOW_HEIGHT - SEQUENCER_PANEL_HEIGHT;
    float seqPanelY           = getHeight() - SEQUENCER_PANEL_HEIGHT - PADDING;
    
    sinesGUI->setBounds(PADDING, PADDING, 230, ENVELOPE_WINDOW_HEIGHT);
    lfoGUI->setBounds(PADDING * 2 + 230 , PADDING, 230, ENVELOPE_WINDOW_HEIGHT);
    ampAhdEnvWindow->setBounds(560, PADDING, 300, ENVELOPE_WINDOW_HEIGHT);
    modAhdEnvWindow->setBounds(870, PADDING, 300, ENVELOPE_WINDOW_HEIGHT);
    stepSeqModule->setBounds ( PADDING, stepSeqModuleY, innerWidth ,stepSeqModuleHeight );
    seqPanel->setBounds(PADDING, seqPanelY, innerWidth, SEQUENCER_PANEL_HEIGHT);
    
}

void FmseqnessAudioProcessorEditor::timerCallback()
{
    stepSeqModule->timerTic();
    lfoGUI->timerTic (processor.getLfoAmp());
//    String message = "CountDown: ";
//    message << processor.portamentoCountDown;
//    DBG(message);
//    String message2 = "Pitch: ";
//    message2 << processor.pitch;
//    DBG(message2);
//    String message3 = "Target Pitch: ";
//    message3 << processor.targetPitch;
//    DBG(message3);
//    String message4 = "LFO Amp: ";
//    message4 << processor.lfoAmp;
//    DBG(message4);
}

void FmseqnessAudioProcessorEditor::actionListenerCallback (const String& message)
{
    if (message == "StepsChanged")
        processor.updateSequncerNumberOfSteps();
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
            lfoGUI->timerTic(0.0f);
        }
    }
}

 
void FmseqnessAudioProcessorEditor::audioProcessorChanged (AudioProcessor* processor) {}

