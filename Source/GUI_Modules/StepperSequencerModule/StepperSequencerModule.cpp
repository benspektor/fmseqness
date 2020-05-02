/*
  ==============================================================================

    StepperSequencerModule.cpp
    Created: 25 Apr 2020 8:31:09pm
    Author:  Ben Spector

  ==============================================================================
*/

#include <JuceHeader.h>
#include "StepperSequencerModule.h"

//==============================================================================
StepperSequencerModule::StepperSequencerModule(StepperSequencerDataModel& dataModel) : mDataModel(dataModel)
{
    String array[] = {"Pitch","Level","Mod Amnt"};
    selector.reset (new AnimatedSelector (array, 3, true));
    selector->addActionListener(this);
    
    pitchController.reset ( new PitchController     ( mDataModel.pitchValues, mDataModel.gateStateValues));
    levelController.reset ( new BarsController      ( false, mDataModel.levelValues, mDataModel.gateStateValues) );
    modController  .reset ( new BarsController      ( true,  mDataModel.modValues, mDataModel.gateStateValues  ) );
    gateStateEditor.reset ( new StepGateStateEditor ( mDataModel.gateStateValues) );
    
    gateStateEditor->addActionListener(this);
    
    addAndMakeVisible (*selector);
    addAndMakeVisible (*gateStateEditor);

    addChildComponent (*modController);
    addChildComponent (*pitchController);
    addChildComponent (*levelController);
    
    pitchController->setVisible(true);

}

StepperSequencerModule::~StepperSequencerModule()
{
}

void StepperSequencerModule::paint (Graphics& g)
{
}

void StepperSequencerModule::resized()
{
    width  = getWidth();
    height = getHeight();
    
    const float controllerHeight = height - PADDING * 2 - SELECTOR_HEIGHT - GATE_EDITOR_HEIGHT;
    
    selector->setBounds(0, 0, width, SELECTOR_HEIGHT);
    pitchController->setBounds ( PADDING, SELECTOR_HEIGHT + PADDING, width - PADDING * 2 , controllerHeight );
    levelController->setBounds ( PADDING, SELECTOR_HEIGHT + PADDING, width - PADDING * 2 , controllerHeight );
    modController  ->setBounds ( PADDING, SELECTOR_HEIGHT + PADDING, width - PADDING * 2 , controllerHeight );
    gateStateEditor->setBounds (0, controllerHeight + SELECTOR_HEIGHT + PADDING * 2, width, GATE_EDITOR_HEIGHT);
}

void StepperSequencerModule::actionListenerCallback (const String& message)
{
    const auto broadcaster = message.upToFirstOccurrenceOf("_", false, true);
    const auto content = message.fromFirstOccurrenceOf("_", false, true);
    
    if (broadcaster == "Selector")
    {
        switchViewTo (static_cast<Controller>(content.getIntValue()) );
    }
    else if (broadcaster == "StepGateStateEditor")
    {
        const auto first = content.upToFirstOccurrenceOf("_", false, true);
        const auto second = content.fromFirstOccurrenceOf("_", false, true);
        pitchController->gateStateChanged (first.getIntValue());
        levelController->gateStateChanged (first.getIntValue());
        modController  ->gateStateChanged (first.getIntValue());
        
        if (second != "")
        {
            pitchController->gateStateChanged (second.getIntValue());
            levelController->gateStateChanged (second.getIntValue());
            modController  ->gateStateChanged (second.getIntValue());
        }
            
    }
}

void StepperSequencerModule::switchViewTo (Controller controllerToDisplay)
{
    displayedController = controllerToDisplay;
    
    switch (controllerToDisplay)
    {
        case Controller::pitchController:
            pitchController->setVisible(true);
            levelController->setVisible(false);
            modController  ->setVisible(false);
            levelController->turnOfSteps();
            modController  ->turnOfSteps();
            break;
            
        case Controller::levelController:
            levelController->setVisible(true);
            pitchController->setVisible(false);
            modController  ->setVisible(false);
            pitchController->turnOfSteps();
            modController  ->turnOfSteps();
            break;
            
        case Controller::modController:
            modController  ->setVisible(true);
            pitchController->setVisible(false);
            levelController->setVisible(false);
            pitchController->turnOfSteps();
            levelController->turnOfSteps();
            break;
    }
}


void StepperSequencerModule::timerTic()
{
    if (isPlaying)
    {
        switch (displayedController)
        {
            case Controller::pitchController:
                return pitchController->timerCallback(currentStep);
                break;
            case Controller::levelController:
                return levelController->timerCallback(currentStep);
                break;
            case Controller::modController:
                return modController->timerCallback(currentStep);
                break;
        }
    }
    
}

void StepperSequencerModule::stop()
{
    isPlaying = false;
    switch (displayedController)
    {
        case Controller::pitchController:
            return pitchController->turnOfSteps();
            break;
        case Controller::levelController:
            return levelController->turnOfSteps();
            break;
        case Controller::modController:
            return modController->turnOfSteps();
            break;
    }
    
}

void StepperSequencerModule::play()
{
    isPlaying = true;
}

void StepperSequencerModule::switchStepTo (int step)
{
    currentStep = step;
}
