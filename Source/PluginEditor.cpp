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
    ampAhdEnvWindow.reset ( new AHDEnvWindow (processor.getAmpAHDEnvDataModel(), "Amp"));
    modAhdEnvWindow.reset ( new AHDEnvWindow (processor.getModAHDEnvDataModel(), "Mod"));
    seqPanel       .reset ( new SequencerPanelModule( processor.getParametersTree() ));
    lfoGUI         .reset ( new LfoGUI ( processor.getParametersTree() ));
    modMatrix      .reset ( new ModulationMatrix (processor.getParametersTree()) );
    
    addAndMakeVisible ( *stepSeqModule );
    addAndMakeVisible ( *modMatrix );
    addAndMakeVisible ( *ampAhdEnvWindow );
    addAndMakeVisible ( *modAhdEnvWindow );
    addAndMakeVisible ( *seqPanel );
    addAndMakeVisible ( *lfoGUI );
    
    setResizable (true, true);
    setSize (1200, 900);
    
    processor.addListener (this);
    processor.addListener (stepSeqModule.get());
    stepSeqModule->addActionListener(this);
    lfoGUI->addActionListener(this);
    processor.addActionListener(this);
    
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
    float stepSeqModuleY      = PADDING * 3 + ENVELOPE_WINDOW_HEIGHT + MOD_MATRIX_HEIGHT;
    float stepSeqModuleHeight = getHeight() - PADDING * 4 - ENVELOPE_WINDOW_HEIGHT - SEQUENCER_PANEL_HEIGHT - MOD_MATRIX_HEIGHT;
    float seqPanelY           = getHeight() - SEQUENCER_PANEL_HEIGHT - PADDING;
    float envWindowWidth      = (innerWidth - PADDING * 2) * (2.0 / 7.0);
    float lfoWindowWidth      = (innerWidth - PADDING * 2) * (3.0 / 7.0);
    float modMatrixY          = PADDING * 2 + ENVELOPE_WINDOW_HEIGHT;
//    sinesGUI->setBounds(PADDING, PADDING, 230, ENVELOPE_WINDOW_HEIGHT);
    
    ampAhdEnvWindow->setBounds (PADDING, PADDING, envWindowWidth, ENVELOPE_WINDOW_HEIGHT);
    lfoGUI         ->setBounds (PADDING * 2 + envWindowWidth , PADDING, lfoWindowWidth, ENVELOPE_WINDOW_HEIGHT);
    modAhdEnvWindow->setBounds (PADDING * 3 + envWindowWidth + lfoWindowWidth, PADDING, envWindowWidth, ENVELOPE_WINDOW_HEIGHT);
    modMatrix      ->setBounds (PADDING, modMatrixY, innerWidth, MOD_MATRIX_HEIGHT);
    stepSeqModule  ->setBounds ( PADDING, stepSeqModuleY, innerWidth ,stepSeqModuleHeight );
    seqPanel       ->setBounds (PADDING, seqPanelY, innerWidth, SEQUENCER_PANEL_HEIGHT);
    
}

void FmseqnessAudioProcessorEditor::timerCallback()
{
    stepSeqModule->timerTic();
    lfoGUI->timerTic (processor.getLfoAmp());
//    String message = "CountDown: ";
//    message << processor.portamentoCountDown;
//    DBG(message);
//    String message2 = "Pitch: ";
//    message2 << processor.calculatedPitch;
//    DBG(message2);
//    String message3 = "Env:       ";
//    message3 << processor.modEnv;
//    DBG(message3);
//    String message4 = "Pitch mod: ";
//    message4 << processor.modMatrix.pitch;
//    DBG(message4);
//    String message5 = "FM mod: ";
//    message5 << processor.modMatrix.fm;
//    DBG(message5);
}

void FmseqnessAudioProcessorEditor::actionListenerCallback (const String& message)
{
    
    if (message == "StepsChanged")
        processor.updateSequncerNumberOfSteps();
    
    else if (message == "LFO Sync Changed")
        processor.updateLFOAngle();
    
    else if (message == "Preset Loaded")
    {
        lfoGUI->loadButtonsState();
        processor.refreshEnvelopesModels();
        ampAhdEnvWindow->loadState();
        modAhdEnvWindow->loadState();
        stepSeqModule->refreshViews();
    }
}

void FmseqnessAudioProcessorEditor::audioProcessorParameterChanged (AudioProcessor* processor,
                                                int parameterIndex,
                                                float newValue)
{
    //play
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

