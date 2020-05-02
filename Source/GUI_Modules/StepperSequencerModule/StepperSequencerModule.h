/*
  ==============================================================================

    StepperSequencerModule.h
    Created: 25 Apr 2020 8:31:09pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AnimatedSelector/AnimatedSelector.h"
#include "StepperSequencerControllers/BarsController.h"
#include "StepperSequencerControllers/PitchController.h"
#include "../../DataModels/StepperSequencerDataModel.h"
#include "StepGateStateEditor/StepGateStateEditor.h"

//==============================================================================
/*
*/

enum Controller
{
    pitchController, levelController, modController };

class StepperSequencerModule    : public Component, public ActionListener
{
public:
    StepperSequencerModule(StepperSequencerDataModel& dataModel);
    ~StepperSequencerModule();

    void paint (Graphics&) override;
    void resized() override;
    
    void actionListenerCallback (const String& message) override;
    
    void switchViewTo (Controller controllerToDisplay);
    void timerTic();
    void stop();
    void play();
    void switchStepTo (int step);
    

private:
    float width = 0.0f, height = 0.0f;
    StepperSequencerDataModel& mDataModel;
    std::unique_ptr<AnimatedSelector> selector;
    std::unique_ptr<PitchController> pitchController;
    std::unique_ptr<BarsController> levelController, modController;
    std::unique_ptr<StepGateStateEditor> gateStateEditor;
    int displayedController = 0;
    int currentStep = 0;
    bool isPlaying = false;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepperSequencerModule)
};
