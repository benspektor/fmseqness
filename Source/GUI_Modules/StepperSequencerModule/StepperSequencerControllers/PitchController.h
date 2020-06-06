/*
  ==============================================================================

    PitchController.h
    Created: 15 Apr 2020 8:10:45pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Bar.h"
#include "BarsMouseArea.h"
#include "../../../DataModels/StepperSequencerDataModel.h"
#include "PitchLineScreen.h"
#include "../StepperSequencerConstants.h"




//==============================================================================
/*
*/
class PitchController    : public Component, public ActionListener,public ActionBroadcaster
{
public:
    
    
    PitchController (PitchControllerValuesModel& dataModel, StepGateStateValuesModel& gateModel);
    ~PitchController();
    
    void paint (Graphics&) override;
    void resized() override;
    
    void addNoteBars();
    void paintPianoRoll (Graphics &g);
    void actionListenerCallback (const String &message) override;
    void timerCallback (int currentStep);
    void lightUpStep (int stepIndex);
    void turnOfSteps();
    void gateStateChanged (int step);
    void calculateNoteLocation (int step, int pitch);
    void refreshView();
    Slider& getSliderRef (int index);
    
private:
    float width = 0.0f, height = 0.0f, barWidth = 0.0f, barHeight = 0.0f;
    Rectangle<float> recs[TOTAL_NUMBER_OF_STEPS];
    String message;
    std::vector<SafePointer<Bar>> bars;
    PitchControllerValuesModel& mPitchDataModel;
    StepGateStateValuesModel& mGateDataModel;
    BarsMouseArea mouseArea;
    PitchLineScreen lineScreen { mPitchDataModel, mGateDataModel, recs };
    Slider sliders[32];
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PitchController)
};
