/*
  ==============================================================================

    StepGateStateEditor.cpp
    Created: 28 Apr 2020 7:18:35pm
    Author:  Ben Spector

  ==============================================================================
*/

#include <JuceHeader.h>
#include "StepGateStateEditor.h"

//==============================================================================
StepGateStateEditor::StepGateStateEditor()
{
}

StepGateStateEditor::~StepGateStateEditor()
{
}



void StepGateStateEditor::paint (Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    
    drawStepRectangles(g);
    drawGateStateIcons(g);
    
    g.setColour(Colours::lightblue);
    auto startRectangleX = PADDING + recWidth * firstStepSlider.getValue();
    auto endRectangeleX = PADDING + recWidth * lastStepSlider.getValue();
    g.fillRect(startRectangleX, 0.0f, recWidth, PADDING);
    g.fillRect(endRectangeleX, height - PADDING, recWidth, PADDING);
    
}

void StepGateStateEditor::resized()
{
    width  = getWidth();
    height = getHeight();
    
    recWidth  = (width - PADDING * 2) / TOTAL_NUMBER_OF_STEPS;
    recHeight = height - PADDING * 2;
}



bool StepGateStateEditor::isClickInStartMarkerZone()
{
    return clickLocation.y < PADDING && clickLocation.x > PADDING && clickLocation.x < width - PADDING;
}

bool StepGateStateEditor::isClickInEndMarkerZone()
{
    return clickLocation.y > height - PADDING && clickLocation.x > PADDING && clickLocation.x < width - PADDING;
}

void StepGateStateEditor::moveFirstStepMarker()
{
    int firstStep = jmin (31.0f, TOTAL_NUMBER_OF_STEPS * (clickLocation.x - PADDING) / (width - PADDING * 2));
    firstStepSlider.setValue(firstStep);
    repaint();
    sendActionMessage("StepGateStateEditor_StepsChanged");
}

void StepGateStateEditor::moveLastStepMarker()
{
    int lastStep = jmin (31.0f, TOTAL_NUMBER_OF_STEPS * (clickLocation.x - PADDING) / (width - PADDING * 2));
    lastStepSlider.setValue(lastStep);
    repaint();
    sendActionMessage("StepGateStateEditor_StepsChanged");
}

void StepGateStateEditor::mouseDrag (const MouseEvent& e)
{
    clickLocation.setXY(e.getPosition().getX(), e.getPosition().getY());
    
    if (isClickInStartMarkerZone())
        moveFirstStepMarker();
    
    else if (isClickInEndMarkerZone())
        moveLastStepMarker();
}

void StepGateStateEditor::sendGateChangeMessage()
{
    int stepNumber = (clickLocation.x - PADDING) / (width - PADDING * 2) * TOTAL_NUMBER_OF_STEPS;
    resetMessage (stepNumber);
    toggleStepGateState (stepNumber);
    sendActionMessage (messege);
    repaint();
}

void StepGateStateEditor::mouseDown (const MouseEvent& e)
{
    clickLocation.setXY(e.getMouseDownX(), e.getMouseDownY());
    
    if (isClickInsideBody(clickLocation))
    {
        if (e.mods.isAltDown())
            changeAllGates();
        else
            sendGateChangeMessage();
    }
    
    else if (isClickInStartMarkerZone())
        moveFirstStepMarker();
    
    else if (isClickInEndMarkerZone())
        moveLastStepMarker();
}

void StepGateStateEditor::changeAllGates()
{
    int stepNumber = (clickLocation.x - PADDING) / (width - PADDING * 2) * TOTAL_NUMBER_OF_STEPS;
    int state = gateSliders[stepNumber].getValue();
    state = state == GATE_OFF ? GATE_ON : state + 1;
    
    if (state == GATE_GLIDE)
        gateSliders[0].setValue(GATE_ON);
    else
        gateSliders[0].setValue(state);

    resetMessage(0);
    sendActionMessage(messege);
    
    for (int stepIndex = 1; stepIndex < MAX_NUM_OF_STEPS; stepIndex++)
    {
        resetMessage (stepIndex);
        gateSliders[stepIndex].setValue (state);
        sendActionMessage(messege);
    }
    repaint();
}

void StepGateStateEditor::drawStepRectangles(Graphics &g)
{
    
    for (auto stepNumber = 0; stepNumber < TOTAL_NUMBER_OF_STEPS; ++stepNumber)
    {
        if ( stepNumber % SIXTEEN_DIVEDER == 0 )
            g.setColour (Colours::lightgrey);
        else
            g.setColour (Colours::grey);

        g.drawRect(PADDING + stepNumber * recWidth, PADDING, recWidth, recHeight);
    }
        
}

void StepGateStateEditor::drawGateStateIcons (Graphics &g)
{
    float gatedLevel = PADDING + 1;
    float glideLevel = PADDING + recHeight / 2;
    float floorLevel = PADDING + recHeight - 1;
    
    float xStartPosition = PADDING + 1;
    
    float leftYPosition  = gatedLevel;
    float rightYPosition = gatedLevel;
    
    g.setColour(Colours::black);
    Path path;
    path.startNewSubPath (xStartPosition, gatedLevel);
    
    for (auto stepNumber = 0; stepNumber < TOTAL_NUMBER_OF_STEPS; ++stepNumber)
    {
        int state = gateSliders[stepNumber].getValue();
        
        switch (state)
        {
            case GATE_ON:
                leftYPosition = gatedLevel;
                break;
                
            case GATE_GLIDE:
                leftYPosition = glideLevel;
                break;
                
            case GATE_OFF:
                leftYPosition = floorLevel;
                break;
        }
        
        
        if (stepNumber == TOTAL_NUMBER_OF_STEPS - 1)
            rightYPosition = floorLevel;
        else
            rightYPosition = gateSliders[stepNumber + 1].getValue() == GATE_GLIDE ? glideLevel : floorLevel;
        
        
        path.lineTo (xStartPosition + stepNumber * recWidth, leftYPosition);
        path.lineTo (xStartPosition + (stepNumber + 1) * recWidth, rightYPosition);
    }
    
    path.lineTo (xStartPosition + TOTAL_NUMBER_OF_STEPS * recWidth, floorLevel);
    path.lineTo (xStartPosition, floorLevel);
    path.closeSubPath();
    g.fillPath(path);
}

bool StepGateStateEditor::isClickInsideBody(juce::Point<float> clickLocation)
{
    return clickLocation.y > PADDING &&
           clickLocation.y < height - PADDING &&
           clickLocation.x > PADDING &&
           clickLocation.x < width - PADDING;
}

void StepGateStateEditor::toggleStepGateState (int stepNumber)
{
    int state =  gateSliders[stepNumber].getValue();
    
    switch (state)
    {
        case GATE_ON:
            if (stepNumber > 0 && gateSliders[stepNumber - 1].getValue() != GATE_OFF)
                gateSliders[stepNumber].setValue(GATE_GLIDE);
            else
                gateSliders[stepNumber].setValue(GATE_OFF);
            break;
            
        case GATE_GLIDE:
            gateSliders[stepNumber].setValue(GATE_OFF);
            break;
            
        case GATE_OFF:
            gateSliders[stepNumber].setValue(GATE_ON);
            break;
    }
    
    if (stepNumber < TOTAL_NUMBER_OF_STEPS - 1 && gateSliders[stepNumber].getValue() == GATE_OFF && gateSliders[stepNumber + 1].getValue() == GATE_GLIDE)
    {
        gateSliders[stepNumber + 1].setValue(GATE_ON);
        messege << "_" << stepNumber + 1;
    }
}

void StepGateStateEditor::resetMessage(int stepNumber)
{
    messege = "";
    messege << "StepGateStateEditor_GateChanged_";
    messege << stepNumber;
}

Slider& StepGateStateEditor::getGateSliderRef (int index)
{
    return gateSliders[index];
}

void StepGateStateEditor::refreshView()
{
    repaint();
}

Slider& StepGateStateEditor::getFirstStepSliderRef ()
{
    return firstStepSlider;
}

Slider& StepGateStateEditor::getLastStepSliderRef  ()
{
    return lastStepSlider;
}
