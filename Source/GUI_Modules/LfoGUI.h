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
class LfoGUI    : public Component, public ActionBroadcaster
{
public:
    LfoGUI (AudioProcessorValueTreeState& parameters);
    ~LfoGUI();

    void paint (Graphics&) override;
    void resized() override;
    
    void timerTic(float value);
    void loadButtonsState();
    

private:
    AudioProcessorValueTreeState& mParameters;
    LFOMeterWindow meterWindow;
    Slider lengthSlider, frequencySlider;
    Label syncLabel { "Sync", "Sync" }, polarityLabel { "Polarity", "Polarity" }, phaseLabel { "Phase", "Phase" }, restartLabel {"Restart", "Restart"};
    LabelButton syncButton { "Sync" }, polarityButton { "BiPolar" }, phaseButton { "0 deg" }, restartButton {"Pattern"};
    std::unique_ptr<SliderAttachment> lengthAttachment;
    std::unique_ptr<SliderAttachment> frequncyAttachment;
    std::vector<SafePointer<TextButton>> shapeButtons;
    
    Value shape      { mParameters.getParameterAsValue ("lfoShape") };
    Value stepSync   { mParameters.getParameterAsValue ("lfoStepSync") };
    Value phase      { mParameters.getParameterAsValue ("lfoPhase") };
    Value lfoRestart { mParameters.getParameterAsValue ("lfoRestart") };
    Value polarity   { mParameters.getParameterAsValue ("lfoPolarity") };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LfoGUI)
};
