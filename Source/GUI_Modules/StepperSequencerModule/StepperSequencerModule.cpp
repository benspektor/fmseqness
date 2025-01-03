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
StepperSequencerModule::StepperSequencerModule(AudioProcessorValueTreeState& parameters) : mParameters(parameters)
{
    String array[] = {"Pitch","FM","Modulator Multi", "Mod Seq"};
    selector.reset (new AnimatedSelector (array, 4, true));
    selector->addActionListener(this);
    
    pitchController   .reset ( new PitchController ( mParameters ));
    fMController      .reset ( new BarsController  ( mParameters, false) );
    multiplyController.reset ( new BarsController  ( mParameters, false, 10));
    modSeqController  .reset ( new BarsController  ( mParameters, true));
    gateStateEditor   .reset ( new StepGateStateEditor());
    
    gateStateEditor->addActionListener(this);
    
    addAndMakeVisible (*selector);
    addAndMakeVisible (*gateStateEditor);

    addChildComponent (*multiplyController);
    addChildComponent (*pitchController);
    addChildComponent (*fMController);
    addChildComponent (*modSeqController);
    
    addAndMakeVisible (leftGreyedOut);
    addAndMakeVisible (rightGreyedOut);
    
    pitchController->setVisible(true);

    leftGreyedOut .setAlpha(0.5);
    rightGreyedOut.setAlpha(0.5);
    
    firstStepAttachment.reset ( new SliderAttachment (mParameters, "firstStepIndex", gateStateEditor->getFirstStepSliderRef()));
    lastStepAttachment.reset  ( new SliderAttachment (mParameters, "lastStepIndex",  gateStateEditor->getLastStepSliderRef()));

    
    for (int i = 0; i < MAX_NUM_OF_STEPS; i++)
    {
        String parameterID {"step"};
        parameterID << i + 1 << "Pitch";
        pitchAttachments[i].reset ( new SliderAttachment (mParameters, parameterID, pitchController->getSliderRef(i)));
    }
    
    for (int i = 0; i < MAX_NUM_OF_STEPS; i++)
    {
        String parameterID {"step"};
        parameterID << i + 1 << "FM";
        fmAttachments[i].reset ( new SliderAttachment (mParameters, parameterID, fMController->getSliderRef(i)));
    }
    
    for (int i = 0; i < MAX_NUM_OF_STEPS; i++)
    {
        String parameterID {"step"};
        parameterID << i + 1 << "ModMulti";
        multiAttachments[i].reset ( new SliderAttachment (mParameters, parameterID, multiplyController->getSliderRef(i)));
    }
    
    for (int i = 0; i < MAX_NUM_OF_STEPS; i++)
    {
        String parameterID {"step"};
        parameterID << i + 1 << "ModSeq";
        modSeqAttachments[i].reset ( new SliderAttachment (mParameters, parameterID, modSeqController->getSliderRef(i)));
    }
    
    for (int i = 0; i < MAX_NUM_OF_STEPS; i++)
    {
        String parameterID {"step"};
        parameterID << i + 1 << "GateState";
        gateAttachments[i].reset ( new SliderAttachment (mParameters, parameterID, gateStateEditor->getGateSliderRef(i)));
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
    modSeqController   ->setBounds ( 0, SELECTOR_HEIGHT + PADDING, width, controllerHeight );
    
    
    gateStateEditor->setBounds ( 0, controllerHeight + SELECTOR_HEIGHT + PADDING * 2, width, GATE_EDITOR_HEIGHT);
    
    drawGreyedOut();
}

void StepperSequencerModule::actionListenerCallback (const String& message)
{
    if (message == "Preset Loaded")
    {
//        pitchController->refreshView();
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
            modSeqController   ->gateStateChanged ( first.getIntValue() );
            
            if (second != "")
            {
                pitchController    ->gateStateChanged ( second.getIntValue() );
                fMController       ->gateStateChanged ( second.getIntValue() );
                multiplyController ->gateStateChanged ( second.getIntValue() );
                modSeqController   ->gateStateChanged ( second.getIntValue() );
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
            modSeqController   ->setVisible ( false );
            fMController       ->turnOffSteps();
            multiplyController ->turnOffSteps();
            modSeqController   ->turnOffSteps();
            modSeqController   ->turnOffSteps();
            break;
            
        case Controller::fmController:
            fMController       ->setVisible ( true  );
            pitchController    ->setVisible ( false );
            multiplyController ->setVisible ( false );
            modSeqController   ->setVisible ( false );
            pitchController    ->turnOfSteps();
            multiplyController ->turnOffSteps();
            modSeqController   ->turnOffSteps();
            break;
            
        case Controller::modulatorMultiController:
            multiplyController ->setVisible ( true  ) ;
            pitchController    ->setVisible ( false );
            fMController       ->setVisible ( false );
            modSeqController   ->setVisible ( false );
            pitchController    ->turnOfSteps();
            fMController       ->turnOffSteps();
            modSeqController   ->turnOffSteps();
            break;
            
        case Controller::seqModController:
            modSeqController ->setVisible  ( true  ) ;
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
                modSeqController->timerCallback(currentStep->load());
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
            modSeqController->turnOffSteps();
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
    pitchController    ->refreshView();
    fMController       ->refreshView();
    multiplyController ->refreshView();
    modSeqController   ->refreshView();
    gateStateEditor    ->refreshView();
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

void StepperSequencerModule::audioProcessorChanged (AudioProcessor* processor, const ChangeDetails& details) {}

