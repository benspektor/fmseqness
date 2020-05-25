/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI_Modules/GUI_Modules.h"

//==============================================================================
/**
*/


class FmseqnessAudioProcessorEditor  : public AudioProcessorEditor, private Timer, private AudioProcessorListener, ActionListener
{
public:
    FmseqnessAudioProcessorEditor (FmseqnessAudioProcessor&);
    ~FmseqnessAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    
    void audioProcessorParameterChanged (AudioProcessor* processor,
                                                    int parameterIndex,
                                                    float newValue) override;

     
    void audioProcessorChanged (AudioProcessor* processor) override;
    
    void actionListenerCallback (const String& message) override;
    
private:
    
    std::unique_ptr<StepperSequencerModule> stepSeqModule;
    std::unique_ptr<FMSinesGUI> sinesGUI;
    std::unique_ptr<AHDEnvWindow> ampAhdEnvWindow, modAhdEnvWindow;
    std::unique_ptr<SequencerPanelModule> seqPanel;
    std::unique_ptr<LfoGUI> lfoGUI;
    
    TooltipWindow tooltipWindow { this }; // instance required for ToolTips to work

    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FmseqnessAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmseqnessAudioProcessorEditor)
};
