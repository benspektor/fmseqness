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
StepGateStateEditor::StepGateStateEditor (StepGateStateValuesModel& dataModel) : gateStatesDataModel(dataModel)
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
}

void StepGateStateEditor::resized()
{
    width  = getWidth();
    height = getHeight();
    
    recWidth  = (width - PADDING * 2) / TOTAL_NUMBER_OF_STEPS;
    recHeight = height - PADDING * 2;
}





void StepGateStateEditor::mouseDown (const MouseEvent& e)
{
    clickLocation.setXY(e.getMouseDownX(), e.getMouseDownY());
    
    if (isClickInsideBody(clickLocation))
    {
        int stepNumber = (clickLocation.x - PADDING) / (width - PADDING * 2) * TOTAL_NUMBER_OF_STEPS;
        resetMessage (stepNumber);
        toggleStepGateState (stepNumber);
        sendActionMessage (messege);
        repaint();
    }
}

void StepGateStateEditor::drawStepRectangles(Graphics &g)
{
    
    for (auto stepNumber = 0; stepNumber < TOTAL_NUMBER_OF_STEPS; ++stepNumber)
    {
        if ( stepNumber % QUARTER_DIVEDER == 0 )
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
    path.startNewSubPath(xStartPosition, gatedLevel);
    
    for (auto stepNumber = 0; stepNumber < TOTAL_NUMBER_OF_STEPS; ++stepNumber)
    {
        switch (gateStatesDataModel.values[stepNumber])
        {
            case on:
                leftYPosition = gatedLevel;
                break;
                
            case glide:
                leftYPosition = glideLevel;
                break;
                
            case off:
                leftYPosition  = floorLevel;
                break;
        }
        
        
        if (stepNumber == TOTAL_NUMBER_OF_STEPS - 1)
            rightYPosition = floorLevel;
        else
            rightYPosition = gateStatesDataModel.values[stepNumber + 1] == glide ? glideLevel : floorLevel;
        
        
        path.lineTo(xStartPosition + stepNumber * recWidth, leftYPosition);
        path.lineTo(xStartPosition + (stepNumber + 1) * recWidth, rightYPosition);
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
    switch (gateStatesDataModel.values[stepNumber])
    {
        case on:
            if (stepNumber > 0 && gateStatesDataModel.values[stepNumber - 1] != off)
                gateStatesDataModel.values[stepNumber] = glide;
            else
                gateStatesDataModel.values[stepNumber] = off;
            break;
            
        case glide:
            gateStatesDataModel.values[stepNumber] = off;
            break;
            
        case off:
            gateStatesDataModel.values[stepNumber] = on;
            break;
    }
    
    if (stepNumber < TOTAL_NUMBER_OF_STEPS - 1 && gateStatesDataModel.values[stepNumber] == off && gateStatesDataModel.values[stepNumber + 1] == glide)
    {
        gateStatesDataModel.values[stepNumber + 1] = on;
        messege << "_" << stepNumber + 1;
    }
}

void StepGateStateEditor::resetMessage(int stepNumber)
{
    messege = "";
    messege << "StepGateStateEditor_";
    messege << stepNumber;
}
