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
    String array[] = {"Pitch","FM","Modulator Multi", "Mod Seq"};
    selector.reset (new AnimatedSelector (array, 4, true));
    selector->addActionListener(this);
    
    pitchController   .reset ( new PitchController     ( mDataModel.pitchValues, mDataModel.gateStateValues));
    fMController      .reset ( new BarsController      ( false, mDataModel.fmValues, mDataModel.gateStateValues) );
    multiplyController.reset ( new BarsController      ( false, mDataModel.modMultiValues, mDataModel.gateStateValues, 10));
    seqModController  .reset ( new BarsController      ( true, mDataModel.seqModValues, mDataModel.gateStateValues));
    gateStateEditor   .reset ( new StepGateStateEditor ( mParameters, mDataModel.gateStateValues) );
    
    gateStateEditor->addActionListener(this);
    
    addAndMakeVisible (*selector);
    addAndMakeVisible (*gateStateEditor);

    addChildComponent (*multiplyController);
    addChildComponent (*pitchController);
    addChildComponent (*fMController);
    addChildComponent (*seqModController);
    
    addAndMakeVisible (leftGreyedOut);
    addAndMakeVisible (rightGreyedOut);
    
    pitchController->setVisible(true);

    leftGreyedOut .setAlpha(0.5);
    rightGreyedOut.setAlpha(0.5);
    
    for (int i = 0; i < MAX_NUM_OF_STEPS; i++)
    {
        String parameterName {"step"};
        parameterName << i + 1 << "Pitch";
        pitchAttachments[i].reset ( new SliderAttachment (mParameters, parameterName, pitchController->getSliderRef(i)));
    }
    
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
    
    pitchController    ->setBounds ( 0, SELECTOR_HEIGHT + PADDING, width, controllerHeight );
    fMController       ->setBounds ( 0, SELECTOR_HEIGHT + PADDING, width, controllerHeight );
    multiplyController ->setBounds ( 0, SELECTOR_HEIGHT + PADDING, width, controllerHeight );
    seqModController   ->setBounds ( 0, SELECTOR_HEIGHT + PADDING, width, controllerHeight );
    
    
    gateStateEditor->setBounds ( 0, controllerHeight + SELECTOR_HEIGHT + PADDING * 2, width, GATE_EDITOR_HEIGHT);
    
    drawGreyedOut();
}

void StepperSequencerModule::actionListenerCallback (const String& message)
{
    if (message == "Preset Loaded")
    {
        DBG("Got it!");
        pitchController->refreshView();
    }
    
    const auto broadcaster = message.upToFirstOccurrenceOf("_", false, true);
    const auto content     = message.fromFirstOccurrenceOf("_", false, true);
    
    if (broadcaster == "Selector")
    {
        switchViewTo (static_cast<Controller>(content.getIntValue()) );
    }
    else if (broadcaster == "StepGateStateEditor")
    {
        const auto action = content.upToFirstOccurrenceOf("_", false, true);
        const auto data   = content.fromFirstOccurrenceOf("_", false, true);
        
        if (action == "GateChanged")
        {
            const auto first  = data.upToFirstOccurrenceOf("_", false, true);
            const auto second = data.fromFirstOccurrenceOf("_", false, true);
            
            pitchController    ->gateStateChanged ( first.getIntValue() );
            fMController       ->gateStateChanged ( first.getIntValue() );
            multiplyController ->gateStateChanged ( first.getIntValue() );
            seqModController   ->gateStateChanged ( first.getIntValue() );
            
            if (second != "")
            {
                pitchController    ->gateStateChanged ( second.getIntValue() );
                fMController       ->gateStateChanged ( second.getIntValue() );
                multiplyController ->gateStateChanged ( second.getIntValue() );
                seqModController   ->gateStateChanged ( second.getIntValue() );
            }
        }
        else if (action == "StepsChanged")
        {
            drawGreyedOut();
            sendActionMessage(action);
        }
    }
}

void StepperSequencerModule::switchViewTo (Controller controllerToDisplay)
{
    displayedController = controllerToDisplay;
    
    switch (controllerToDisplay)
    {
        case Controller::pitchController:
            pitchController    ->setVisible ( true  );
            fMController       ->setVisible ( false );
            multiplyController ->setVisible ( false );
            seqModController   ->setVisible ( false );
            fMController       ->turnOffSteps();
            multiplyController ->turnOffSteps();
            seqModController   ->turnOffSteps();
            seqModController   ->turnOffSteps();
            break;
            
        case Controller::fmController:
            fMController       ->setVisible ( true  );
            pitchController    ->setVisible ( false );
            multiplyController ->setVisible ( false );
            seqModController   ->setVisible ( false );
            pitchController    ->turnOfSteps();
            multiplyController ->turnOffSteps();
            seqModController   ->turnOffSteps();
            break;
            
        case Controller::modulatorMultiController:
            multiplyController ->setVisible ( true  ) ;
            pitchController    ->setVisible ( false );
            fMController       ->setVisible ( false );
            seqModController   ->setVisible ( false );
            pitchController    ->turnOfSteps();
            fMController       ->turnOffSteps();
            seqModController   ->turnOffSteps();
            break;
            
        case Controller::seqModController:
            seqModController ->setVisible  ( true  ) ;
            multiplyController->setVisible ( false );
            pitchController   ->setVisible ( false );
            fMController      ->setVisible ( false );
            pitchController   ->turnOfSteps();
            fMController      ->turnOffSteps();
            multiplyController->turnOffSteps();
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
            case Controller::fmController:
                fMController->timerCallback(currentStep->load());
                break;
            case Controller::modulatorMultiController:
                multiplyController->timerCallback(currentStep->load());
                break;
            case Controller::seqModController:
                seqModController->timerCallback(currentStep->load());
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
            pitchController->turnOfSteps();
            break;
        case Controller::fmController:
            fMController->turnOffSteps();
            break;
        case Controller::modulatorMultiController:
            multiplyController->turnOffSteps();
            break;
        case Controller::seqModController:
            seqModController->turnOffSteps();
            break;
    }
    
}

void StepperSequencerModule::play()
{
    isPlaying = true;
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

void StepperSequencerModule::refreshViews()
{
    pitchController->refreshView();
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

