/*
  ==============================================================================

    AnimatedSelector.cpp
    Created: 22 Apr 2020 7:37:52pm
    Author:  Ben Spector

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AnimatedSelector.h"

//==============================================================================

AnimatedSelector::AnimatedSelector(String labels[], size_t size, bool isHorizontal) 
{
    this->isHorizontal = isHorizontal;
    this->numOfOptions = size;
    for (auto i = 0; i < size; ++i)
    {
        mLabels.push_back(labels[i]);
        labelsRectangles.push_back(Rectangle<float>());
    }
}

AnimatedSelector::~AnimatedSelector()
{
}

void AnimatedSelector::paint (Graphics& g)
{
    
    g.fillAll (backgroundColour);   // clear the background

    if (shouldDrawBorder)
    {
        g.setColour (borderColour);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    }
    
    for (auto i = 0; i < numOfOptions; ++i)
    {
        g.setColour (labelColour);
        g.fillRect (labelsRectangles[i]);
    }
    
    g.setColour (highlightColour);
    g.fillRect (highlightRectangle);
    

    for (auto i = 0; i < numOfOptions; ++i)
    {
        g.setColour (fontColour);
        g.setFont (fontSize);
        g.drawText (mLabels[i], labelsRectangles[i], Justification::centred, true);
    }

}

void AnimatedSelector::resized()
{
    labelWidth = isHorizontal ? (getWidth() - (numOfOptions + 1) * padding) / numOfOptions : getWidth() - 2 * padding;
    labelHeight = isHorizontal ? getHeight() - padding * 2 : (getHeight() - (numOfOptions + 1) * padding) / numOfOptions;
    
    for (auto i = 0; i < labelsRectangles.size(); ++i)
    {
        if (isHorizontal)
            labelsRectangles[i].setBounds (padding + (labelWidth + padding) * i, padding, labelWidth, labelHeight);
        
        else
            labelsRectangles[i].setBounds (padding, padding + (labelHeight + padding) * i, labelWidth, labelHeight);
    }
    
    highlightRectangle.setBounds(labelsRectangles[currentOption].getX(), labelsRectangles[currentOption].getY(), labelWidth, labelHeight);
    
    animationUnit = isHorizontal ? getWidth() / animationsSteps : getHeight() / animationsSteps;
}

void AnimatedSelector::mouseDown  (const MouseEvent& event)
{
    for (auto i = 0; i < labelsRectangles.size(); ++i)
    {
        if (labelsRectangles[i].contains(event.x, event.y))
        {
            if (currentOption != i)
            {
                currentOption = i;
                startAnimation();
            }
            break;
        }
    }
}

void AnimatedSelector::startAnimation()
{
    if (isHorizontal)
    {
        if (labelsRectangles[currentOption].getX() >= highlightRectangle.getX())
            animationSign = 1.0f;
        else
            animationSign = -1.0f;
    }
    else
    {
        if (labelsRectangles[currentOption].getY() >= highlightRectangle.getY())
            animationSign = 1.0f;
        else
            animationSign = -1.0f;
    }
    
    startTimerHz (animationsFrequency);
}

void AnimatedSelector::stopAnimation()
{
    highlightRectangle.setBounds(labelsRectangles[currentOption].getX(), labelsRectangles[currentOption].getY(), labelWidth, labelHeight);
    sendMessage();
    repaint();
    stopTimer();
}

void AnimatedSelector::timerCallback()
{
    if (shouldAnimationFinish())
        stopAnimation();
    
    if (isTimerRunning())
    {
        if (isHorizontal)
             highlightRectangle.setX (highlightRectangle.getX() + animationUnit * animationSign);
        else
            highlightRectangle.setY (highlightRectangle.getY() + animationUnit * animationSign);
        repaint();
    }
    
}

bool AnimatedSelector::shouldAnimationFinish()
{
    if (isHorizontal)
        return (animationSign == 1.0f && labelsRectangles[currentOption].getX() <= highlightRectangle.getX()) ||
               (animationSign == -1.0f && labelsRectangles[currentOption].getX() >= highlightRectangle.getX());
    else
        return (animationSign == 1.0f && labelsRectangles[currentOption].getY() <= highlightRectangle.getY()) ||
               (animationSign == -1.0f && labelsRectangles[currentOption].getY() >= highlightRectangle.getY());
}

void AnimatedSelector::sendMessage()
{
    message = "Selector_";
    message << currentOption;
    sendActionMessage(message);
}

void AnimatedSelector::setColours (Colour backgroundColour, Colour labelColour, Colour highlightColour, Colour fontColour, Colour borderColour)
{
    this->backgroundColour = backgroundColour;
    this->labelColour = labelColour;
    this->highlightColour = highlightColour;
    this->fontColour = fontColour;
    this->borderColour = borderColour;
}

void AnimatedSelector::setFontSize (float size)
{
    fontSize = size;
}
