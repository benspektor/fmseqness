/*
  ==============================================================================

    BasePitchPicker.h
    Created: 12 May 2020 11:36:36am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Supporting_Files/SupportingFiles.h"
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
        basePitchPickerBox.addItemList (BASE_NOTES, MINIMUM_PITCH);
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
        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
        g.setColour (Colours::white);
        g.setFont (14.0f);
        Rectangle<float> labelRect { 0, 0, static_cast<float>(getWidth()), static_cast<float>(getHeight() - PADDING / 2) };
        g.drawText ("Base Pitch", labelRect, Justification::centredBottom, true);   // draw some placeholder text
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
