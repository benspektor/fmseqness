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
#include "StepGateStateEditor/StepGateStateEditor.h"
#include "GreyScreen.h"

//==============================================================================
/*
*/

enum Controller
{
    pitchController, fmController, modulatorMultiController, seqModController
};

class StepperSequencerModule : public Component, public ActionListener, public AudioProcessorListener, public ActionBroadcaster
{
public:
    StepperSequencerModule(AudioProcessorValueTreeState& parameters);
    ~StepperSequencerModule();

    void paint (Graphics&) override;
    void resized() override;
    
    //AudioProcessorListener
    void audioProcessorParameterChanged (AudioProcessor* processor,
                                         int parameterIndex,
                                         float newValue) override;
    void audioProcessorChanged (AudioProcessor* processor) override;
    
    
    void actionListenerCallback (const String& message) override;
    
    void switchViewTo (Controller controllerToDisplay);
    void timerTic();
    void stop();
    void play();
    void switchStepTo (int step);
    void drawGreyedOut();
    void refreshViews();
    

private:
    float width = 0.0f, height = 0.0f, barLength = 0.0f, leftScreenX = 0.0f, leftScreenWidth = 0.0f, rightScreenX = 0.0f, rightScreenWidth = 0.0f;
    
    AudioProcessorValueTreeState& mParameters;
    
    std::atomic<float>* currentStep     { mParameters.getRawParameterValue("currentStep") };
    std::atomic<float>* firstStepIndex  { mParameters.getRawParameterValue("firstStepIndex") };
    std::atomic<float>* lastStepIndex   { mParameters.getRawParameterValue("lastStepIndex") };
    
    std::unique_ptr<AnimatedSelector>    selector;
    std::unique_ptr<PitchController>     pitchController;
    std::unique_ptr<BarsController>      fMController, multiplyController, modSeqController;
    std::unique_ptr<StepGateStateEditor> gateStateEditor;
    
    std::unique_ptr<SliderAttachment> pitchAttachments [MAX_NUM_OF_STEPS];
    std::unique_ptr<SliderAttachment> fmAttachments    [MAX_NUM_OF_STEPS];
    std::unique_ptr<SliderAttachment> multiAttachments [MAX_NUM_OF_STEPS];
    std::unique_ptr<SliderAttachment> modSeqAttachments[MAX_NUM_OF_STEPS];
    std::unique_ptr<SliderAttachment> gateAttachments  [MAX_NUM_OF_STEPS];
    std::unique_ptr<SliderAttachment> firstStepAttachment, lastStepAttachment;
    
    int displayedController = 0;
    bool isPlaying = false;
    
    GreyScreen leftGreyedOut, rightGreyedOut;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StepperSequencerModule)
};
