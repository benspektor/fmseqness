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
#include "../Supporting_Files/SupportingFiles.h"


//==============================================================================
/*
*/
class AHDEnvWindow : public Component
{
public:
    AHDEnvWindow (AHDEnvDataModel& mModel, String name);
    ~AHDEnvWindow();

    void paint (Graphics&) override;
    void resized() override;
        
    void updateModel();
    void loadState();
    
    Slider& getSlider (int sliderIndex);

private:
    void mouseDrag (const MouseEvent& e) override;
    void mouseUp (const MouseEvent&) override;
    

    EnvelopeControl currentDraging = EnvelopeControl::none;
    
    const float lineWidth  = 1;
    const float frameWidth = 4;
    const float dotSize    = 20;
    const float padding    = 16;
    
    AHDEnvDataModel& mModel;
    
    Slider sliders[6];
    std::unique_ptr<SliderAttachment> attachments[6];
    
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
