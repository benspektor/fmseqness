/*
  ==============================================================================

    FMSinesGUI.h
    Created: 9 Feb 2020 1:26:11pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../DSP_Modules/FM2SineOscsGenerator.h"

//==============================================================================
/*
*/
class FMSinesGUI    : public Component
{
public:
    FMSinesGUI (AudioProcessorValueTreeState& parameters);
    ~FMSinesGUI();

    void paint (Graphics&) override;
    void resized() override;
    

private:
    AudioProcessorValueTreeState& mParameters;
    Slider fmAmountKnob, modMultiKnob;
    Label fmAmountLabel, modMultiLabel;
    std::unique_ptr<SliderAttachment> fmAmountAttachment;
    std::unique_ptr<SliderAttachment> modMultiAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMSinesGUI)
};
