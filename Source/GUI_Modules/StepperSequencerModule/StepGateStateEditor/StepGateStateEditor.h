/*
  ==============================================================================

    StepGateStateEditor.h
    Created: 28 Apr 2020 7:18:35pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../../DataModels/StepperSequencerDataModel.h"
#include "../StepperSequencerConstants.h"
#include "../../../Supporting_Files/Constants.h"



//==============================================================================
/*
*/
class StepGateStateEditor    : public Component, public ActionBroadcaster
{
public:
    StepGateStateEditor();
    ~StepGateStateEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    void drawGateStateIcons(Graphics &g);
    void drawStepRectangles(Graphics &g);
    bool isClickInsideBody(juce::Point<float> clickLocation);
    void toggleStepGateState (int stepNumber);
    void resetMessage (int stepNumber);
    bool isClickInStartMarkerZone();
    bool isClickInEndMarkerZone();
    void moveLastStepMarker();
    void moveFirstStepMarker();
    void sendGateChangeMessage();
    void changeAllGates();
    void refreshView();
    
    Slider& getGateSliderRef (int index);
    Slider& getFirstStepSliderRef();
    Slider& getLastStepSliderRef();
    
    void mouseDown (const MouseEvent& e) override;
    void mouseDrag (const MouseEvent& e) override;

private:
//    AudioProcessorValueTreeState& mParameters;
    String messege = "";
    float width, height, recWidth, recHeight;
    juce::Point<float> clickLocation;
    Slider gateSliders[MAX_NUM_OF_STEPS];
    Slider firstStepSlider, lastStepSlider;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepGateStateEditor)
};
