/*
  ==============================================================================

    BarsController.cpp
    Created: 9 Apr 2020 11:36:07pm
    Author:  Ben Spector

  ==============================================================================
*/

#include <JuceHeader.h>
#include "BarsController.h"



//==============================================================================
BarsController::BarsController(bool isBidirectional, BarsControllerValuesModel& dataModel, StepGateStateValuesModel& gateModel, int numOfDiscreteValues) :  mGateModel(gateModel), mDataModel(dataModel)
{
    this->isBidirectional = isBidirectional;
    this->numOfDiscreteValues = numOfDiscreteValues;
    
    addControllBars();
    
    addAndMakeVisible (mouseArea);
    mouseArea.addActionListener (this);
    
}

BarsController::~BarsController()
{
}

void BarsController::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    g.setColour(Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
}



void BarsController::resized()
{
    width  = getWidth();
    height = getHeight();
   
    barWidth = (width - PADDING * 2) / TOTAL_NUMBER_OF_STEPS;
    barMaxHeight = isBidirectional ? (height - PADDING * 2) / 2 : (height - PADDING * 2);
    
    resizeControllBars();
    mouseArea.setBounds(PADDING, PADDING, width - PADDING * 2, height - PADDING * 2);
}

void BarsController::addControllBars()
{
    for (int stepNumber = 0; stepNumber < TOTAL_NUMBER_OF_STEPS; ++stepNumber)
    {
        bool isDownBeat = stepNumber % QUARTER_DIVEDER == 0 ? true : false;
        SafePointer<Bar> bar = new Bar(isDownBeat);
        bars.push_back (bar);
        addChildComponent (bar);
        if (mGateModel.values[stepNumber] == on)
            bar->setVisible(true);
    }
}



void BarsController::resizeControllBars()
{
    for (int stepNumber = 0; stepNumber < TOTAL_NUMBER_OF_STEPS; ++stepNumber)
        resizeControllBar (stepNumber);
}

void BarsController::resizeControllBar (int stepNumber)
{
    auto height = barMaxHeight * abs(mDataModel.values[stepNumber]);
    auto y = barMaxHeight - height + PADDING;
    y = mDataModel.values[stepNumber] >= 0 ? y : barMaxHeight + PADDING;
    bars[stepNumber]->setBounds ( PADDING + stepNumber * barWidth, y, barWidth, height);
}

void BarsController::actionListenerCallback(const String &message)
{
    const auto broadcaster = message.upToFirstOccurrenceOf("_", false, true);
    const auto content     = message.fromFirstOccurrenceOf("_", false, true);
    
    if (broadcaster == "BarsMouseArea")
    {
        const auto barNumber = content.upToFirstOccurrenceOf("_", false, true).getIntValue();
        auto value = content.fromFirstOccurrenceOf("_", false, true).getFloatValue();
        
        if (numOfDiscreteValues > 1)
        {
            int areas = numOfDiscreteValues * 2 - 2;
            int currentArea = value * areas;
            
            if (currentArea == 0)
            {
                value = 0.0f;
            }
            else if (currentArea == areas - 1)
            {
                value = 1.0f;
            }
            else
            {
                currentArea = (currentArea + 1) / 2;
                value = float(currentArea) / float (numOfDiscreteValues - 1);
            }
        }
        
        value = isBidirectional ? value * 2 - 1 : value;
        
        if (barNumber == MAX_NUM_OF_STEPS)
        {
            for (int barIndex = 0; barIndex < MAX_NUM_OF_STEPS; barIndex++)
            {
                mDataModel.values[barIndex] = value;
                resizeControllBar (barIndex);
            }
        }
        else
        {
            mDataModel.values[barNumber] = value;
            resizeControllBar (barNumber);
        }
        
        
        this->message = "BarsController_";
        this->message << content;
        sendActionMessage(this->message);
    }
}

void BarsController::lightUpStep (int stepIndex)
{
    turnOffSteps();
    bars[stepIndex]->lightUp();
}

void BarsController::turnOffSteps()
{
    for (Bar *bar : bars)
        bar->turnOff();
}

void BarsController::timerCallback ( int currentStep )
{
    turnOffSteps();

    for (int i = 0; i < 32; ++i)
    {
        if (i == currentStep)
        {
            if (mGateModel.values[i] == on)
            {
                bars[i]->lightUp();
            }
            else if (mGateModel.values[i] == glide)
            {
                auto gate = i;
                
                while (mGateModel.values[gate] != on)
                    gate--;
                
                bars[gate]->lightUp();
            }
        }
            
    }
}

void BarsController::gateStateChanged (int step)
{
    switch (mGateModel.values[step])
    {
        case on:
            bars[step]->setVisible(true);
            break;
            
        case off:
        case glide:
            bars[step]->setVisible(false);
    }
}

