/*
  ==============================================================================

    AHDEnvWindow.h
    Created: 26 Feb 2020 10:32:36am
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../DataModels/AHDEnvDataModel.h"
#include "../Supporting_Files/Constants.h"
#include "../Supporting_Files/Enums.h"

//==============================================================================
/*
*/
class AHDEnvWindow : public Component, public ActionBroadcaster
{
public:
    AHDEnvWindow (AHDEnvDataModel& mModel, String name);
    ~AHDEnvWindow();

    void paint (Graphics&) override;
    void resized() override;
        
    void updateModel();
    

private:
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent&) override;
    
    float convert0to1toCurveValue (float value);

    EnvelopeControl currentDraging = EnvelopeControl::none;
    
    const float lineWidth  = 1;
    const float frameWidth = 4;
    const float dotSize    = 20;
    const float padding    = 16;
    
    
    AHDEnvDataModel& mModel;
    
    Rectangle<float> startRect;
    Rectangle<float> attackRect;
    Rectangle<float> holdRect;
    Rectangle<float> decayRect;
    Rectangle<float> attackCurveRect;
    Rectangle<float> decayCurveRect;
    
    float attack, hold, decay, level;
    
    String mName;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AHDEnvWindow)
};
