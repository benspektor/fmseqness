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
PitchController::PitchController (AudioProcessorValueTreeState& parameters) : mParameters(parameters)
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
    g.setColour(Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    paintPianoRoll(g);
}

void PitchController::resized()
{
    width     = getWidth();
    height    = getHeight();
    barWidth  = (width - PADDING * 2) / TOTAL_NUMBER_OF_STEPS;
    barHeight = (height - PADDING * 2) / PIANO_ROLL_LENGTH;
    
    for (int stepNumber = 0; stepNumber < TOTAL_NUMBER_OF_STEPS; ++stepNumber)
    {
        int  value     = sliders[stepNumber].getValue() - OCTAVE;
        auto yPosition = -1 * value * barHeight + PADDING + 1;
        auto xPsition  = PADDING + stepNumber * barWidth;
        recs[stepNumber].setBounds  ( xPsition, yPosition, barWidth, barHeight);
        bars[stepNumber]->setBounds ( xPsition, yPosition, barWidth, barHeight);
    }
    
    lineScreen.setBounds (0, 0, width, height);
    mouseArea.setBounds  (PADDING, PADDING, width - PADDING * 2, height - PADDING * 2);
    

}

void PitchController::paintPianoRoll(juce::Graphics &g)
{
    for (int lineIndex = 0; lineIndex < PIANO_ROLL_LENGTH; lineIndex++)
    {
        switch (lineIndex)
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
        
        auto rect = Rectangle<float>(PADDING, PADDING + lineIndex * barHeight, getWidth() - PADDING * 2, barHeight);
        g.fillRect(rect);
        g.setColour((Colour((uint8)0, (uint8)0, (uint8)0, 0.3f)));
        g.drawLine (PADDING, PADDING + lineIndex * barHeight, getWidth() - PADDING, PADDING + lineIndex * barHeight);
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
        
        if (*mParameters.getRawParameterValue(STEPS_GATE[stepNumber]) != GATE_OFF)
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
                
        //Change all bars at ones
        if (bar == MAX_NUM_OF_STEPS)
        {
            for (int stepIndex = 0; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
            {
                sliders[stepIndex].setValue(pitch);
                calculateNoteLocation(stepIndex, pitch);
            }
        }
        else
        {
            sliders[bar].setValue(pitch);
            calculateNoteLocation(bar, pitch);
        }
       
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
    switch ((int)*mParameters.getRawParameterValue(STEPS_GATE[stepNumber]))
    {
        case GATE_ON: case GATE_GLIDE:
            bars[stepNumber]->setVisible(true);
            break;
        case GATE_OFF:
            bars[stepNumber]->setVisible(false);
    }
    
    lineScreen.repaint();
}

Slider& PitchController::getSliderRef (int index)
{
    return sliders[index];
}

void PitchController::calculateNoteLocation (int step, int pitch)
{
    auto yPosition = -1 * (pitch - 12) * barHeight + PADDING + 1;
    auto xPsition  = PADDING + step * barWidth;
    bars[step]->setBounds (xPsition, yPosition, barWidth, barHeight);
    recs[step].setBounds  (xPsition, yPosition, barWidth, barHeight);
}

void PitchController::refreshView()
{
    for (int step = 0; step < MAX_NUM_OF_STEPS; step++)
    {
        auto pitch = sliders[step].getValue();
        calculateNoteLocation(step, pitch);
    }
    repaint();
}

