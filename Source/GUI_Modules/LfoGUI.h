/*
  ==============================================================================

    LfoGUI.h
    Created: 20 May 2020 10:26:01am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "LFOMeterWindow.h"
#include "../Supporting_Files/Constants.h"

//==============================================================================
/*
*/
class LfoGUI    : public Component
{
public:
    LfoGUI (AudioProcessorValueTreeState& parameters);
    ~LfoGUI();

    void paint (Graphics&) override;
    void resized() override;
    
    void timerTic(float value);

private:
    AudioProcessorValueTreeState& mParameters;
    LFOMeterWindow meterWindow;
    Slider lengthSlider;
    std::unique_ptr<SliderAttachment> lengthAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LfoGUI)
};
