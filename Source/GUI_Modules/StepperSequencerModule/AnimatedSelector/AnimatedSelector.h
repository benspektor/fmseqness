/*
  ==============================================================================

    AnimatedSelector.h
    Created: 22 Apr 2020 7:37:52pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AnimatedSelector    : public Component, public ActionBroadcaster, private Timer
{
public:
    AnimatedSelector(String labels[], size_t size, bool isHorizontal = false);
    ~AnimatedSelector();

    void paint (Graphics&) override;
    void resized() override;
    
    void mouseDown  (const MouseEvent&) override;
    
    void startAnimation();
    void stopAnimation();
    void timerCallback() override;
    
    bool shouldAnimationFinish();
    void sendMessage();
    void setColours (Colour backgroundColour,
                     Colour labelColour,
                     Colour highlightColour,
                     Colour fontColour,
                     Colour borderColour);
    void setFontSize (float size);
    

private:
    bool isHorizontal = false;
    int currentOption = 0;
    size_t numOfOptions = 3;
    float padding = 10.0f;
    float labelWidth = 1.0f;
    float labelHeight = 1.0f;
    float animationUnit = 0.0f;
    float animationSign = 1.0f;
    std::vector<String> mLabels;
    std::vector<Rectangle<float>> labelsRectangles;
    Rectangle<float> highlightRectangle;
    String message;
    Colour backgroundColour = Colour(323e44), labelColour = Colours::black, highlightColour = Colour(200,171,0), fontColour = Colours::white, borderColour = Colours::grey;
    float fontSize = 22.0f;
    bool shouldDrawBorder = true;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnimatedSelector)
};
