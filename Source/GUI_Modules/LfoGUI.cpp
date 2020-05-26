/*
  ==============================================================================

    LfoGUI.cpp
    Created: 20 May 2020 10:26:01am
    Author:  Ben Spector

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LfoGUI.h"

//==============================================================================
LfoGUI::LfoGUI (AudioProcessorValueTreeState& parameters) : mParameters(parameters)
{
    addAndMakeVisible(meterWindow);
    
    addChildComponent            (lengthSlider);
    lengthSlider.setSliderStyle  (Slider::SliderStyle::IncDecButtons);
    lengthSlider.setTextBoxStyle (Slider::TextBoxAbove, false, 60.0, 20.0);
    lengthSlider.setTextValueSuffix (" Steps");
    lengthAttachment.reset       ( new SliderAttachment (mParameters, "LfoLength", lengthSlider ));
    
    addChildComponent               (frequencySlider);
    frequencySlider.setSliderStyle  (Slider::SliderStyle::RotaryVerticalDrag);
    frequencySlider.setTextBoxStyle (Slider::TextBoxAbove, false, 60.0, 20.0);
    frequencySlider.setTextValueSuffix ( " Hz" );
    frequncyAttachment.reset        ( new SliderAttachment (mParameters, "lfoFrequency", frequencySlider ));
    
    addAndMakeVisible(polarityButton);
    polarityButton.changeName (LFO_POLARITIES[polarity->load()]);
    
    polarityButton.onClick = [this]
    {
        int oldPolarity = polarity->load();
        int newPolarity = oldPolarity == 2.0f ? 0.0 : oldPolarity + 1;
        polarity->store (newPolarity);
        polarityButton.changeName (LFO_POLARITIES[polarity->load()]);
    };
    
    addAndMakeVisible(syncButton);
    
    if (stepSync->load())
    {
        syncButton.changeName   ( "Step Sync" );
        lengthSlider.setVisible ( true );
    }
    else
    {
        syncButton.changeName      ( "Free" );
        frequencySlider.setVisible ( true );
    }
    
    
    syncButton.onClick = [this]
    {
        if (stepSync->load())
        {
            stepSync->store(0.0f);
            syncButton.changeName("Free");
            lengthSlider.setVisible(false);
            frequencySlider.setVisible(true);
        }
        else
        {
            stepSync->store(1.0f);
            syncButton.changeName("Step Sync");
            lengthSlider.setVisible(true);
            frequencySlider.setVisible(false);
        }
        sendActionMessage ("LFO Sync Changed");
    };
    
    
    for (int i = 0; i < 4; ++i)
    {
        auto label = LFO_SHAPES[i];
        
        SafePointer<TextButton> tb = new TextButton (label);
        
        addAndMakeVisible (tb);
        
        tb->setClickingTogglesState (true);
        tb->setRadioGroupId (34567);
        tb->setColour (TextButton::textColourOffId,  Colours::white);
        tb->setColour (TextButton::textColourOnId,   Colours::black);
        tb->setColour (TextButton::buttonColourId,   Colours::black);
        tb->setColour (TextButton::buttonOnColourId, Colour(200,171,0));
        tb->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
        
        tb->onClick = [this, i]
        {
            shape->store(i);
        };
        
        shapeButtons.push_back(tb);

        if (i == 0)
            tb->setToggleState (true, dontSendNotification);
    }

    

}

LfoGUI::~LfoGUI()
{
}

void LfoGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::grey);
    g.setFont (30.0f);
    g.drawText ("LFO", Rectangle<float> (PADDING, PADDING, getWidth(), getHeight()),
                Justification::topLeft, true);   // draw some placeholder text
}

void LfoGUI::resized()
{
    float meterWindowWidth     = getWidth() / 6.0f;
    float meterWindowX         = getWidth() - PADDING - meterWindowWidth;
    float shapeButtonWidth     = (getWidth() - PADDING * 3 - meterWindowWidth) / 4.0f;
    float shapeButtonHeight    = 30;
    float shapeButtonY         = getHeight() - PADDING - shapeButtonHeight;
    float frequencySliderWidth = 80;
    float frequencySliderX     = getWidth() - PADDING * 2 - meterWindowWidth - frequencySliderWidth;
    float syncButtonX          = frequencySliderX;
    float syncButtonY          = getHeight() - PADDING * 2 - shapeButtonHeight * 2;
    
    
    lengthSlider   .setBounds (frequencySliderX + PADDING, PADDING * 2, frequencySliderWidth - PADDING * 2, 50);
    frequencySlider.setBounds (frequencySliderX, PADDING, frequencySliderWidth, 80);
    meterWindow    .setBounds (meterWindowX, PADDING, meterWindowWidth, getHeight() - PADDING * 2);
    syncButton     .setBounds (syncButtonX, syncButtonY, frequencySliderWidth, shapeButtonHeight);
    polarityButton .setBounds (PADDING, syncButtonY, frequencySliderWidth, shapeButtonHeight);
    
    for (int i = 0; i < 4; ++i)
        shapeButtons[i]->setBounds (PADDING + i * shapeButtonWidth, shapeButtonY, shapeButtonWidth, shapeButtonHeight);

}

void LfoGUI::timerTic(float value)
{
    meterWindow.drawMeter(value);
}
