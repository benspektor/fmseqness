/*
  ==============================================================================

    BarsController.h
    Created: 9 Apr 2020 11:36:07pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Bar.h"
#include "BarsMouseArea.h"
#include "../../../DataModels/StepperSequencerDataModel.h"
#include "../StepperSequencerConstants.h"

//==============================================================================
/*
*/
class BarsController    : public Component, public ActionListener, public ActionBroadcaster
{
public:
    void extracted();
    
    BarsController(bool isBidirectional, BarsControllerValuesModel& model, StepGateStateValuesModel& gateModel, int numOfSteps = 0);
    ~BarsController();
    
    void paint (Graphics&) override;
    void resized() override;
    
    void resizeControllBar(int stepNumber);
    
    void resizeControllBars();
    void actionListenerCallback(const String &message) override;
    void timerCallback (int currentStep);
    void addControllBars();
    void lightUpStep (int stepIndex);
    void turnOffSteps();
    void gateStateChanged (int step);

private:
    float width = 0.0f, height = 0.0f;
    float barWidth = 0.0f;
    float barMaxHeight = 0.0f;
    bool isBidirectional = false;
    int numOfDiscreteValues = 0;
    String message;
    std::vector<SafePointer<Bar>> bars;
    StepGateStateValuesModel& mGateModel;
    BarsControllerValuesModel& mDataModel;
    BarsMouseArea mouseArea;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BarsController)
};
