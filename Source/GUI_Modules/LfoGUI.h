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
#include "SequencerPanelModule/LabelButton.h"

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
    Slider lengthSlider, frequencySlider;
    LabelButton syncButton {"Step Sync"};
    std::unique_ptr<SliderAttachment> lengthAttachment;
    std::unique_ptr<SliderAttachment> frequncyAttachment;
    std::vector<SafePointer<TextButton>> shapeButtons;
    std::atomic<float>* shape    { mParameters.getRawParameterValue ("lfoShape") };
    std::atomic<float>* stepSync { mParameters.getRawParameterValue ("lfoStepSync") };
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LfoGUI)
};
