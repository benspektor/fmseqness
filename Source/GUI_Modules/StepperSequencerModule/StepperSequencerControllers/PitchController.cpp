/*
  ==============================================================================

    PitchController.cpp
    Created: 15 Apr 2020 8:10:45pm
    Author:  Ben Spector

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PitchController.h"



//==============================================================================
PitchController::PitchController (PitchControllerValuesModel& dataModel, StepGateStateValuesModel& gateModel) : mPitchDataModel(dataModel), mGateDataModel(gateModel)
{
    addNoteBars();
    addAndMakeVisible (lineScreen);
    addAndMakeVisible (mouseArea);
    mouseArea.addActionListener (this);
}

PitchController::~PitchController()
{
}

void PitchController::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    paintPianoRoll(g);
}

void PitchController::resized()
{
    width     = getWidth();
    height    = getHeight();
    barWidth  = width / TOTAL_NUMBER_OF_STEPS;
    barHeight = height / PIANO_ROLL_LENGTH;
    
    for (int stepNumber = 0; stepNumber < TOTAL_NUMBER_OF_STEPS; ++stepNumber)
    {
        auto yPosition = -1 * (mPitchDataModel.values[stepNumber] - OCTAVE) * barHeight;
        recs[stepNumber].setBounds ( stepNumber * barWidth, yPosition, barWidth, barHeight);
        bars[stepNumber]->setBounds ( stepNumber * barWidth, yPosition, barWidth, barHeight);
    }
    
    lineScreen.setBounds (0, 0, width, height);
    mouseArea.setBounds  (0, 0, width, height);
    

}

void PitchController::paintPianoRoll(juce::Graphics &g)
{
    for (int line = 0; line < PIANO_ROLL_LENGTH; line++)
    {
        switch (line)
        {
            case 0: case 12: case 24:
                g.setColour((Colour((uint8)255, (uint8)255, (uint8)255, 0.1f)));
                break;
            case 1: case 3: case 5: case 7: case 8: case 10: case 13: case 15: case 17: case 19: case 20: case 22:
                g.setColour((Colour((uint8)255, (uint8)255, (uint8)255, 0.05f)));
                break;
            default:
                g.setColour((Colour((uint8)0, (uint8)0, (uint8)0, 0.05f)));
                break;
        }
        
        auto rect = Rectangle<float>(0, line * barHeight, getWidth(), barHeight);
        g.fillRect(rect);
        g.setColour((Colour((uint8)0, (uint8)0, (uint8)0, 0.3f)));
        g.drawLine(0, line * barHeight, getWidth(), line * barHeight);
    }
}

void PitchController::addNoteBars()
{
    for (int stepNumber = 0; stepNumber < TOTAL_NUMBER_OF_STEPS; ++stepNumber)
    {
        bool isDownBeat = stepNumber % QUARTER_DIVEDER == 0 ? true : false;
        SafePointer<Bar> bar = new Bar(isDownBeat);
        bars.push_back (bar);
        addChildComponent (bar);
        
        if (mGateDataModel.values[stepNumber] != off)
            bar->setVisible(true);
    }
}

void PitchController::actionListenerCallback(const String &message)
{
    const auto broadcaster = message.upToFirstOccurrenceOf("_", false, true);
    const auto content     = message.fromFirstOccurrenceOf("_", false, true);
    
    if (broadcaster == "BarsMouseArea")
    {
        const auto bar = content.upToFirstOccurrenceOf("_", false, true).getIntValue();
        auto value = content.fromFirstOccurrenceOf("_", false, true).getFloatValue();
        
        value = jlimit(-11.99f, 12.0f, value * PIANO_ROLL_LENGTH - OCTAVE);
        int pitch = value < 0 ? value - 1 : value;
        mPitchDataModel.values[bar] = pitch;
        auto y = -1 * (pitch - 12) * barHeight;
        bars[bar]->setBounds(bar * barWidth, y, barWidth, barHeight);
        recs[bar].setBounds(bar * barWidth, y, barWidth, barHeight);
        lineScreen.repaint();
        this->message = "PitchController_";
        this->message << bar << "_" << pitch;
        sendActionMessage(this->message);
    }
    
}


void PitchController::lightUpStep ( int stepIndex )
{
    turnOfSteps();
    bars[stepIndex]->lightUp();
}

void PitchController::turnOfSteps()
{
    for (Bar *bar : bars)
        bar->turnOff();
}

void PitchController::timerCallback ( int currentStep )
{
    turnOfSteps();
    
    for (int i = 0; i < TOTAL_NUMBER_OF_STEPS; ++i)
        if (i == currentStep)
            bars[i]->lightUp();
}

void PitchController::gateStateChanged (int stepNumber)
{
    switch (mGateDataModel.values[stepNumber])
    {
        case on: case glide:
            bars[stepNumber]->setVisible(true);
            break;
        case off:
            bars[stepNumber]->setVisible(false);
    }
    
    lineScreen.repaint();
}

