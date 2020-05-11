/*
  ==============================================================================

    SequencerPanelModule.h
    Created: 11 May 2020 9:34:48am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PlayStopButton.h"
#include "../../Supporting_Files/Constants.h"
#include "SwingController.h"


//==============================================================================
/*
*/
class SequencerPanelModule    : public Component
{
public:
    SequencerPanelModule (AudioProcessorValueTreeState& parameters);
    ~SequencerPanelModule();

    void paint (Graphics&) override;
    void resized() override;
    

private:
    AudioProcessorValueTreeState& mParameters;
    PlayStopButton playStopButton;
    
    std::unique_ptr<ButtonAttachment> playStopAttachment;
    
    SwingController swingController {mParameters};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SequencerPanelModule)
};
