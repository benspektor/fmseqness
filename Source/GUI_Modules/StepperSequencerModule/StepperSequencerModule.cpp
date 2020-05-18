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
StepperSequencerModule::StepperSequencerModule(StepperSequencerDataModel& dataModel, AudioProcessorValueTreeState& parameters) : mDataModel(dataModel), mParameters(parameters)
{
    String array[] = {"Pitch","Level","Mod Amnt"};
    selector.reset (new AnimatedSelector (array, 3, true));
    selector->addActionListener(this);
    
   
    
    pitchController.reset ( new PitchController     ( mDataModel.pitchValues, mDataModel.gateStateValues));
    levelController.reset ( new BarsController      ( false, mDataModel.levelValues, mDataModel.gateStateValues) );
    modController  .reset ( new BarsController      ( true,  mDataModel.modValues, mDataModel.gateStateValues  ) );
    gateStateEditor.reset ( new StepGateStateEditor ( mParameters, mDataModel.gateStateValues) );
    
    gateStateEditor->addActionListener(this);
    
    addAndMakeVisible (*selector);
    addAndMakeVisible (*gateStateEditor);

    addChildComponent (*modController);
    addChildComponent (*pitchController);
    addChildComponent (*levelController);
    
    addAndMakeVisible (leftGreyedOut);
    addAndMakeVisible (rightGreyedOut);
    
    pitchController->setVisible(true);

    leftGreyedOut.setAlpha(0.5);
    rightGreyedOut.setAlpha(0.5);
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
    barLength = (width - PADDING * 2) / TOTAL_NUMBER_OF_STEPS;
    
    const float controllerHeight = height - PADDING * 3 - SELECTOR_HEIGHT - GATE_EDITOR_HEIGHT;
    
    selector->setBounds(0, 0, width, SELECTOR_HEIGHT);
    
    pitchController->setBounds ( 0, SELECTOR_HEIGHT + PADDING, width, controllerHeight );
    levelController->setBounds ( 0, SELECTOR_HEIGHT + PADDING, width, controllerHeight );
    modController  ->setBounds ( 0, SELECTOR_HEIGHT + PADDING, width, controllerHeight );
    
    
    gateStateEditor->setBounds ( 0, controllerHeight + SELECTOR_HEIGHT + PADDING * 2, width, GATE_EDITOR_HEIGHT);
    
    drawGreyedOut();
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
        const auto action = content.upToFirstOccurrenceOf("_", false, true);
        const auto data = content.fromFirstOccurrenceOf("_", false, true);
        
        if (action == "GateChanged")
        {
            const auto first = data.upToFirstOccurrenceOf("_", false, true);
            const auto second = data.fromFirstOccurrenceOf("_", false, true);
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
        else if (action == "StepsChanged")
        {
            drawGreyedOut();
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
                pitchController->timerCallback(currentStep->load());
                break;
            case Controller::levelController:
                levelController->timerCallback(currentStep->load());
                break;
            case Controller::modController:
                modController->timerCallback(currentStep->load());
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
//    currentStep = step->load();
}

void StepperSequencerModule::drawGreyedOut()
{
    auto firstStepIndex = this->firstStepIndex->load();
    auto lastStepIndex  = this->lastStepIndex ->load();
    
    auto screenY      = SELECTOR_HEIGHT + PADDING * 2;
    auto screenHeight = height - PADDING * 5 - SELECTOR_HEIGHT - GATE_EDITOR_HEIGHT;
    
    
    if (firstStepIndex <= lastStepIndex)
    {
        leftScreenX      = PADDING;
        leftScreenWidth  = barLength * firstStepIndex;
        rightScreenX     = PADDING + barLength * (lastStepIndex + 1);
        rightScreenWidth = barLength * (TOTAL_NUMBER_OF_STEPS - lastStepIndex - 1);
    }
    else
    {
        leftScreenX      = 0.0f;
        leftScreenWidth  = 0.0f;
        rightScreenX     = PADDING + barLength * (lastStepIndex + 1);
        rightScreenWidth = barLength * (firstStepIndex - lastStepIndex - 1);
    }
    
    
    leftGreyedOut.setBounds  (leftScreenX, screenY, leftScreenWidth, screenHeight);
    rightGreyedOut.setBounds (rightScreenX, screenY, rightScreenWidth, screenHeight);
    
    
}

void StepperSequencerModule::audioProcessorParameterChanged (AudioProcessor* processor,
                                     int parameterIndex,
                                     float newValue)
{
    switch (parameterIndex)
    {
        case 6: case 7:
            drawGreyedOut();
            break;
        default:
            break;
    }
}

void StepperSequencerModule::audioProcessorChanged (AudioProcessor* processor) {}

