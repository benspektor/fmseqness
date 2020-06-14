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
    
    addChildComponent               ( lengthSlider );
    lengthSlider.setSliderStyle     ( Slider::SliderStyle::IncDecButtons );
    lengthSlider.setTextBoxStyle    ( Slider::TextBoxAbove, false, 60.0, 20.0 );
    lengthSlider.setTextValueSuffix ( " Steps" );
    lengthAttachment.reset          ( new SliderAttachment ( mParameters, "LfoLength", lengthSlider ));
    
    addChildComponent                  ( frequencySlider );
    frequencySlider.setSliderStyle     ( Slider::SliderStyle::RotaryVerticalDrag );
    frequencySlider.setTextBoxStyle    ( Slider::TextBoxAbove, false, 60.0, 20.0 );
    frequencySlider.setTextValueSuffix (  " Hz" );
    frequncyAttachment.reset           (  new SliderAttachment (mParameters, "lfoFrequency", frequencySlider ));
    
    addAndMakeVisible ( polarityButton );
    addAndMakeVisible ( phaseButton );
    addAndMakeVisible ( syncButton );
    addAndMakeVisible ( restartButton );
    
    for (int i = 0; i < 4; ++i)
    {
        auto label = LFO_SHAPES[i];
        
        SafePointer<TextButton> shapeButton = new TextButton (label);
        
        addAndMakeVisible (shapeButton);
        
        shapeButton->setClickingTogglesState (true);
        shapeButton->setRadioGroupId (34567);
        shapeButton->setColour (TextButton::textColourOffId,  Colours::white);
        shapeButton->setColour (TextButton::textColourOnId,   Colours::black);
        shapeButton->setColour (TextButton::buttonColourId,   Colours::black);
        shapeButton->setColour (TextButton::buttonOnColourId, Colour(200,171,0));
        shapeButton->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
        
        shapeButton->onClick = [this, i] { shape = i; };
        
        shapeButtons.push_back(shapeButton);
    }
    
    loadButtonsState();
    
    polarityButton.onClick = [this]
    {
        int oldPolarity = polarity.getValue();
        int newPolarity = oldPolarity == 2.0f ? 0.0 : oldPolarity + 1;
        polarity = newPolarity;
        polarityButton.changeName (LFO_POLARITIES[polarity.getValue()]);
    };
    
    phaseButton.onClick = [this]
    {
        float currentPhase = phase.getValue();
        
        if (currentPhase == 0.0f)
        {
            phase = 0.25f;
            phaseButton.changeName("90 deg");
        }
        else if (currentPhase == 0.25f)
        {
            phase = 0.5f;
            phaseButton.changeName("180 deg");
        }
        else if (currentPhase == 0.5f)
        {
            phase = 0.75f;
            phaseButton.changeName("270 deg");
        }
        else if (currentPhase == 0.75f)
        {
            phase = 0.0f;
            phaseButton.changeName("0 deg");
        }
    };
    
    syncButton.onClick = [this]
    {
        if (stepSync.getValue())
        {
            stepSync = 0.0f;
            syncButton.changeName("Free");
            lengthSlider.setVisible(false);
            frequencySlider.setVisible(true);
        }
        else
        {
            stepSync = 1.0f;
            syncButton.changeName("Step Sync");
            lengthSlider.setVisible(true);
            frequencySlider.setVisible(false);
        }
        sendActionMessage ("LFO Sync Changed");
    };
    
    
    restartButton.onClick = [this]
    {
        int restart = lfoRestart.getValue();
        
        if (restart == 2)
            restart = 0;
        else
            restart += 1;
        
        lfoRestart = restart;
        
        restartButton.changeName ( LFO_RESTART_OPTIONS[restart] );
    };
    
    addAndMakeVisible (syncLabel);
    addAndMakeVisible (polarityLabel);
    addAndMakeVisible (phaseLabel);
    addAndMakeVisible (restartLabel);
    
    syncLabel    .setJustificationType (Justification::centred);
    polarityLabel.setJustificationType (Justification::centred);
    phaseLabel   .setJustificationType (Justification::centred);
    restartLabel .setJustificationType (Justification::centred);
    

}

LfoGUI::~LfoGUI()
{
}

void LfoGUI::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::grey);
    g.setFont (30.0f);
    g.drawText ("LFO", Rectangle<float> (PADDING, PADDING, getWidth() - PADDING * 2, getHeight()),
                Justification::centredTop, true);   // draw some placeholder text
}

void LfoGUI::resized()
{
    float meterWindowWidth     = getWidth() / 5.0f;
    float meterWindowX         = getWidth() - PADDING - meterWindowWidth;
    float shapeButtonWidth     = getWidth() / 5.0f;;
    float shapeButtonHeight    = (getHeight() - PADDING * 2) / 4.0f;
    float frequencySliderWidth = getWidth() / 4.0f;
    float frequencySliderX     = (getWidth() - frequencySliderWidth) / 2.0f;
    float frequencySliderY     = 50;
    float labelButtonWidth     = (getWidth() - meterWindowWidth - shapeButtonWidth - PADDING * 4) / 4.0f;
    float labelButtonHeight    = getHeight() / 8.0f;
    float labelButtonY         = getHeight() - PADDING - labelButtonHeight;
    float syncButtonX          = PADDING * 2 + shapeButtonWidth;
    float labelY               = labelButtonY - labelButtonHeight;
    
    
    lengthSlider   .setBounds (frequencySliderX + PADDING, frequencySliderY + PADDING, frequencySliderWidth - PADDING * 2, 50);
    frequencySlider.setBounds (frequencySliderX, frequencySliderY, frequencySliderWidth, 80);
    meterWindow    .setBounds (meterWindowX, PADDING, meterWindowWidth, getHeight() - PADDING * 2);
    syncButton     .setBounds (syncButtonX, labelButtonY, labelButtonWidth, labelButtonHeight);
    polarityButton .setBounds (syncButtonX + labelButtonWidth, labelButtonY, labelButtonWidth, labelButtonHeight);
    phaseButton    .setBounds (syncButtonX + labelButtonWidth * 2, labelButtonY, labelButtonWidth, labelButtonHeight);
    restartButton  .setBounds (syncButtonX + labelButtonWidth * 3, labelButtonY, labelButtonWidth, labelButtonHeight);
    syncLabel      .setBounds (syncButtonX, labelY, labelButtonWidth, labelButtonHeight);
    polarityLabel  .setBounds (syncButtonX + labelButtonWidth, labelY, labelButtonWidth, labelButtonHeight);
    phaseLabel     .setBounds (syncButtonX + labelButtonWidth * 2, labelY, labelButtonWidth, labelButtonHeight);
    restartLabel   .setBounds (syncButtonX + labelButtonWidth * 3, labelY, labelButtonWidth, labelButtonHeight);
    
    for (int i = 0; i < 4; ++i)
        shapeButtons[i]->setBounds (PADDING, PADDING + shapeButtonHeight * i, shapeButtonWidth, shapeButtonHeight);

}

void LfoGUI::timerTic(float value)
{
    meterWindow.drawMeter(value);
}

void LfoGUI::loadButtonsState()
{
    shape      = mParameters.getParameterAsValue ("lfoShape");
    stepSync   = mParameters.getParameterAsValue ("lfoStepSync");
    phase      = mParameters.getParameterAsValue ("lfoPhase");
    lfoRestart = mParameters.getParameterAsValue ("lfoRestart");
    polarity   = mParameters.getParameterAsValue ("lfoPolarity");
    
    polarityButton.changeName ( LFO_POLARITIES[polarity.getValue()] );
    restartButton .changeName ( LFO_RESTART_OPTIONS[int(lfoRestart.getValue())] );
    
    float currentPhase = phase.getValue();
    
    if (currentPhase == 0.0f)
        phaseButton.changeName("0 deg");
    else if (currentPhase == 0.25f)
        phaseButton.changeName("90 deg");
    else if (currentPhase == 0.5f)
        phaseButton.changeName("180 deg");
    else if (currentPhase == 0.75f)
        phaseButton.changeName("270 deg");
    
    if (stepSync.getValue())
    {
        syncButton.changeName      ( "Step Sync" );
        lengthSlider.setVisible    ( true );
        frequencySlider.setVisible ( false );
    }
    else
    {
        syncButton.changeName      ( "Free" );
        frequencySlider.setVisible ( true );
        lengthSlider.setVisible    ( false );
    }
    
    for (int i = 0; i < 4; ++i)
    {
        if (i == (int)shape.getValue())
            shapeButtons[i]->setToggleState (true, dontSendNotification);
        else
            shapeButtons[i]->setToggleState (false, dontSendNotification);
    }
}
