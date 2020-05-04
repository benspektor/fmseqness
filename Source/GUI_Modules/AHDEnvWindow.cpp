/*
  ==============================================================================

    AHDEnvWindow.cpp
    Created: 26 Feb 2020 10:32:36am
    Author:  Ben Spector

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AHDEnvWindow.h"

//==============================================================================
AHDEnvWindow::AHDEnvWindow (AHDEnvDataModel& model, String name) : mModel(model), mName(name)
{
}

AHDEnvWindow::~AHDEnvWindow()
{
}

void AHDEnvWindow::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.drawRoundedRectangle(padding, padding, getWidth() - padding * 2, getHeight() - padding * 2, 5, frameWidth);
    
    g.setFont (30.0f);
    g.drawText (mName, getLocalBounds(), Justification::centred, true);
    
    g.setColour (Colours::white);
    
    const float startX    = frameWidth + padding + dotSize / 2;
    const float startY    = getHeight() - startX;

    float const attackLengthDisplayValue = attackRect.getCentreX() - startX;
    float const attackHeightDisplayValue = startY - attackRect.getCentreY();
    
    Path attackPath;
    attackPath.startNewSubPath (startX, startY);
    
    for (int i  = 10; i >= 0; --i)
    {
        float y = i/10.0 ;
        float x = pow(y,mModel.attackCurve);
        y = y * attackHeightDisplayValue + attackRect.getCentreY() - lineWidth / 2;
        x = (1 - x) * attackLengthDisplayValue - lineWidth / 2 + startX;
        attackPath.lineTo(x, y);
    }
    
    attackPath = attackPath.createPathWithRoundedCorners(500);
    g.strokePath(attackPath, PathStrokeType(1));
    
    
    g.drawLine (attackRect.getCentreX(), attackRect.getCentreY(), holdRect.getCentreX(), attackRect.getCentreY(), lineWidth);
    
    
    float const decayLengthDisplayValue = decayRect.getCentreX() - holdRect.getCentreX();
    float const decayHeightDisplayValue = startY - attackRect.getCentreY();
    
    Path decayPath;
    decayPath.startNewSubPath (holdRect.getCentreX(), attackRect.getCentreY());
    
    for (int i  = 0; i <= 10; ++i)
    {
        float y = i/10.0 ;
        float x = pow(y,mModel.decayCurve);
        y = y * decayHeightDisplayValue + attackRect.getCentreY() - lineWidth / 2;
        x = x * decayLengthDisplayValue - lineWidth / 2 + holdRect.getCentreX();
        decayPath.lineTo(x, y);
    }
    decayPath = decayPath.createPathWithRoundedCorners(500);
    g.strokePath(decayPath, PathStrokeType(1));
    
    //Draw Dots
    g.setColour(Colour { 0x88000000 });
    g.fillEllipse (startRect);
    g.fillEllipse (attackRect);
    g.fillEllipse (holdRect);
    g.fillEllipse (decayRect);
    g.fillEllipse (attackCurveRect);
    g.fillEllipse (decayCurveRect);
}

void AHDEnvWindow::resized()
{
    startRect       .setSize (dotSize, dotSize);
    attackRect      .setSize (dotSize, dotSize);
    holdRect        .setSize (dotSize, dotSize);
    decayRect       .setSize (dotSize, dotSize);
    attackCurveRect .setSize (dotSize, dotSize);
    decayCurveRect  .setSize (dotSize, dotSize);
    
    const float totalPadding = frameWidth + padding + dotSize / 2;
    const float envHeight = getHeight() - totalPadding * 2;
    const float envWidth  = getWidth() - totalPadding * 2;
    const float startX    = totalPadding;
    const float startY    = getHeight() - totalPadding;
    const float attackX   = totalPadding + mModel.attack * envWidth;
    const float attackY   = getHeight() - totalPadding - envHeight * mModel.level;
    const float holdX     = attackX + envWidth * mModel.hold;
    const float holdY     = padding;
    const float decayX    = holdX + envWidth * mModel.hold;
    const float decayY    = startY;
    
    const float attackCurveX = padding;
    const float attackCurveY = getHeight() / 2;
    const float decacyCurveX = getWidth() - padding;
    const float decayCurveY  = getHeight() / 2;
    
    startRect       .setCentre (startX, startY);
    attackRect      .setCentre (attackX, attackY);
    holdRect        .setCentre (holdX, holdY);
    decayRect       .setCentre (decayX, decayY);
    attackCurveRect .setCentre (attackCurveX, attackCurveY);
    decayCurveRect  .setCentre (decacyCurveX, decayCurveY);
}


void AHDEnvWindow::mouseUp (const MouseEvent&)
{
    currentDraging = EnvelopeControl::none;
}

void AHDEnvWindow::mouseDrag (const MouseEvent& e)
{
    float x = e.position.getX();
    float y = e.position.getY();
    
    const float totalPadding = frameWidth + padding + dotSize / 2;
    const float startY       = getHeight() - totalPadding;
    const float holdY        = padding;
    const float decayY       = startY;
    const float attackCurveX = padding;
    const float decacyCurveX = getWidth() - padding;
    
    //attack drag
    if ((e.position.getDistanceFrom(attackRect.getCentre()) < dotSize / 2
         || currentDraging == EnvelopeControl::attack))
    {
        currentDraging = EnvelopeControl::attack;
        const float minX = totalPadding;
        const float maxX = holdRect.getCentreX();
        const float apliedX = std::min(maxX, std::max(minX, x));
        const float minY = totalPadding;
        const float maxY = getHeight() - totalPadding;
        const float apliedY = std::min(maxY, std::max(minY, y));
        attackRect.setCentre(apliedX, apliedY);
    }
    //Hold drag
    else if ((e.position.getDistanceFrom(holdRect.getCentre()) < dotSize / 2 ||
              currentDraging == EnvelopeControl::hold))
    {
        currentDraging = EnvelopeControl::hold;
        const float holdDecayDiff = decayRect.getCentreX() - holdRect.getCentreX();
        const float minX = attackRect.getCentreX();
        const float maxX = getWidth() - totalPadding;
        const float apliedX = std::min(maxX, std::max(minX, x));
        holdRect.setCentre(apliedX, holdY);
        const float apliedDecayX = std::min(apliedX + holdDecayDiff, maxX);
        decayRect.setCentre(apliedDecayX, decayY);
    }
    //decay drag
    else if ((e.position.getDistanceFrom(decayRect.getCentre()) < dotSize / 2 ||
              currentDraging == EnvelopeControl::decay))
    {
        currentDraging = EnvelopeControl::decay;
        const float minX = holdRect.getCentreX();
        const float maxX = getWidth() - totalPadding;
        const float apliedX = std::min(maxX, std::max(minX, x));
        decayRect.setCentre(apliedX, decayY);
    }
    //Attack curve drag
    else if ((e.position.getDistanceFrom(attackCurveRect.getCentre()) < dotSize / 2 ||
              currentDraging == EnvelopeControl::attackCurve))
    {
        currentDraging = EnvelopeControl::attackCurve;
        const float minY = totalPadding;
        const float maxY = getHeight() - totalPadding;
        const float apliedY = std::min(maxY, std::max(minY, y));
        attackCurveRect.setCentre(attackCurveX, apliedY);
    }
    //Decay curve drag
    else if ((e.position.getDistanceFrom(decayCurveRect.getCentre()) < dotSize / 2 ||
              currentDraging == EnvelopeControl::decayCurve))
    {
        currentDraging = EnvelopeControl::decayCurve;
        const float minY = totalPadding;
        const float maxY = getHeight() - totalPadding;
        const float apliedY = std::min(maxY, std::max(minY, y));
        decayCurveRect.setCentre(decacyCurveX, apliedY);
    }
    else
    {
        return;
    }
    repaint();
    updateModel();
    
}

void AHDEnvWindow::updateModel()
{
    const float totalPadding = frameWidth + padding + dotSize / 2;
    
    const float width  = getWidth() - (padding + frameWidth) * 2 - dotSize;
    const float height = getHeight() - (padding + frameWidth) * 2 - dotSize;
    
    mModel.attack = (attackRect.getCentreX() - totalPadding) / width;
    mModel.hold   = (holdRect.getCentreX() - totalPadding) / width - mModel.attack;
    mModel.decay  = (decayRect.getCentreX() - totalPadding) / width - mModel.attack - mModel.hold;
    mModel.level  = 1 - (attackRect.getCentreY() - totalPadding) / height;
    
    const float attackCurve = (attackCurveRect.getCentreY() - totalPadding) / height;
    const float decayCurve  = (decayCurveRect.getCentreY()  - totalPadding) / height;
    
    mModel.attackCurve = convert0to1toCurveValue (attackCurve);
    mModel.decayCurve  = convert0to1toCurveValue (decayCurve);
    
}

float AHDEnvWindow::convert0to1toCurveValue (float value)
{
    return value <= 0.5 ? value + 0.5 : 1.0 + (value - 0.5) * 14.0;
}



