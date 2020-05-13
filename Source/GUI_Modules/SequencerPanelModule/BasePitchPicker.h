/*
  ==============================================================================

    BasePitchPicker.h
    Created: 12 May 2020 11:36:36am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Supporting_Files/Constants.h"
#include "LabelButton.h"


//==============================================================================
/*
*/
class BasePitchPicker    : public Component
{
public:
    BasePitchPicker (AudioProcessorValueTreeState& parameters) : mParameters (parameters)
    {
        addAndMakeVisible (basePitchPickerBox);
        StringArray notes
        {
            "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
            "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
            "C3", "C#3", "D3", "D#3", "E3", "F3", "F#3", "G3", "G#3", "A3", "A#3", "B3",
            "C4", "C#4", "D4", "D#4", "E4", "F4", "F#4", "G4", "G#4", "A4", "A#4", "B4",
            "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5", "A#5", "B5", "C6"
        };
        
        basePitchPickerBox.addItemList (notes, MINIMUM_PITCH);
        basePitchPickerBox.onChange = [this]() { basePitch->store(basePitchPickerBox.getSelectedId()); };
        basePitchPickerBox.setSelectedId (BASE_PITCH);
        
        addAndMakeVisible (plusButton);
        addAndMakeVisible (minusButton);
        
        minusButton.onClick = [this]()
        {
            int currentID = basePitchPickerBox.getSelectedId();
            
            if (currentID > MINIMUM_PITCH)
            {
                basePitchPickerBox.setSelectedId (currentID - 1);
            }
        };
        
        plusButton.onClick = [this]()
        {
            int currentID = basePitchPickerBox.getSelectedId();
            
            if (currentID < MAXIMUM_PITCH)
            {
                basePitchPickerBox.setSelectedId (currentID + 1);
            }
        };
        
        
    }

    ~BasePitchPicker()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

//        g.setColour (Colours::grey);
//        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

        g.setColour (Colours::white);
        g.setFont (14.0f);
        g.drawText ("Base Pitch", Rectangle<float>(0, 0, getWidth(), getHeight() - PADDING / 2),
                    Justification::centredBottom, true);   // draw some placeholder text
    }

    void resized() override
    {
        float innerWidth   = getWidth() - PADDING * 2;
        float boxHeight    = getHeight() / 3.0f;
        float buttonWidth  = innerWidth / 2.0f;
        float buttonHeight = getHeight() / 4.0f;
        
        basePitchPickerBox.setBounds (PADDING, PADDING, innerWidth, boxHeight);
        minusButton.setBounds (PADDING, boxHeight + PADDING, buttonWidth, buttonHeight);
        plusButton.setBounds  (PADDING + buttonWidth, boxHeight + PADDING, buttonWidth, buttonHeight);
    }

private:
    AudioProcessorValueTreeState& mParameters;
    std::atomic<float>* basePitch { mParameters.getRawParameterValue ("basePitch") };
    ComboBox basePitchPickerBox;
    LabelButton plusButton {"+"}, minusButton {"-"};
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasePitchPicker)
};
