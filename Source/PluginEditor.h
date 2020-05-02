/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI_Modules/StepperSequencerModule/StepperSequencerModule.h"

//==============================================================================
/**
*/
class FmseqnessAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    FmseqnessAudioProcessorEditor (FmseqnessAudioProcessor&);
    ~FmseqnessAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    std::unique_ptr<StepperSequencerModule> stepSeqModule;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FmseqnessAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FmseqnessAudioProcessorEditor)
};
