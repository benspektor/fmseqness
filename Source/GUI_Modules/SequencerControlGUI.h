/*
  ==============================================================================

    SequencerControlGUI.h
    Created: 9 Feb 2020 9:49:16pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../DSP_Modules/StepSequencer.h"
#include "../Supporting_Files/Constants.h"
//#include "SequencerStepsGUI.h"

//==============================================================================
/*
*/
class SequencerControlGUI    : public Component
{
public:
    SequencerControlGUI(AudioProcessorValueTreeState& parameters);
    ~SequencerControlGUI();

    void paint (Graphics&) override;
    void resized() override;
    
private:
    AudioProcessorValueTreeState& mParameters;
    Slider tempoKnob, numOfStepsKnob;
    ToggleButton playButton;
    std::unique_ptr<SliderAttachment> tempoAttachment;
    std::unique_ptr<SliderAttachment> stepsAttachment;
    std::unique_ptr<ButtonAttachment> playAttachment;
    Label tempoLabel, numOfStepsLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SequencerControlGUI)
};
