/*
  ==============================================================================

    BarsMouseArea.h
    Created: 10 Apr 2020 5:56:02pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../../Supporting_Files/Constants.h"

//==============================================================================
/*
*/
class BarsMouseArea    : public Component, public ActionBroadcaster
{
public:
    BarsMouseArea()
    {
    }

    ~BarsMouseArea()
    {
    }

    void paint (Graphics& g) override
    {
    }

    void resized() override
    {
        width = getWidth();
        height = getHeight();
    }
    
    void mouseDrag (const MouseEvent& e) override
    {
        const float x = e.position.getX();
        const float y = e.position.getY();
        int barNumber = 0;
       
        if (e.mods.isAltDown())
            barNumber = TOTAL_NUMBER_OF_STEPS;
        else
            barNumber = jlimit(0.0f, float(TOTAL_NUMBER_OF_STEPS - 1), x / width * TOTAL_NUMBER_OF_STEPS);
        
        const float value = jlimit(0.0f, 1.0f, 1 - y / height);
        
        message = "BarsMouseArea_";
        message << barNumber << "_" << value;
        sendActionMessage(message);
        
    }

private:
    String message;
    float width, height;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BarsMouseArea)
};
