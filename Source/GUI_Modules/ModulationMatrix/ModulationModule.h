/*
  ==============================================================================

    ModulationModule.h
    Created: 26 May 2020 10:54:06pm
    Author:  Ben Spector

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../../Supporting_Files/Constants.h"

//==============================================================================
/*
*/
class ModulationModule    : public Component
{
public:
    ModulationModule(int index, AudioProcessorValueTreeState& parameters) : mIndex (index), mParameters (parameters) 
    {
        name << index;
        
        addAndMakeVisible(sources);
        String code1 {name};
        code1 << "Source";
        sourcesAttachment.reset ( new ComboBoxAttachment(mParameters, code1, sources) );
        sources.addItemList(MODULATION_SOURCES, 1);
        std::atomic<float>* source { mParameters.getRawParameterValue (code1) };
        sources.setSelectedItemIndex (int(source->load()));
        
        addAndMakeVisible(amountSlider);
        amountSlider.setDoubleClickReturnValue(true, 0.0f);
        amountSlider.setTextBoxStyle(Slider::NoTextBox, true, 0, 0);
        amountSlider.setSliderStyle(juce::Slider::SliderStyle::LinearHorizontal);
        String code2 {name};
        code2 << "Amount";
        amountAttachment.reset (new SliderAttachment (mParameters, code2, amountSlider));
        
        addAndMakeVisible(destinations);
        String code3 {name};
        code3 << "Destination";
        destinationAttachment.reset ( new ComboBoxAttachment(mParameters, code3, destinations) );
        destinations.addItemList(MODULATION_DESTINATIONS, 1);
        std::atomic<float>* destination { mParameters.getRawParameterValue (code3) };
        destinations.setSelectedItemIndex (int(destination->load()));
        
    }

    ~ModulationModule()
    {
    }

    void paint (Graphics& g) override
    {
        /* This demo code just fills the component's background and
           draws some placeholder text to get you started.

           You should replace everything in this method with your own
           drawing code..
        */

        g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

        g.setColour (Colours::grey);
        g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

//        g.setColour (Colours::white);
//        g.setFont (14.0f);
//        g.drawText ("ModulationModule", getLocalBounds(),
//                    Justification::centred, true);   // draw some placeholder text
    }

    void resized() override
    {
        float boxWidth = (getWidth() - PADDING * 4) * (2.0 / 7.0);
        float sliderWidth = (getWidth() - PADDING * 4) * (3.0 / 7.0);
        
        sources.setBounds (PADDING, PADDING, boxWidth, getHeight() - PADDING * 2);
        amountSlider.setBounds (PADDING * 2 + boxWidth, PADDING, sliderWidth, getHeight() - PADDING * 2);
        destinations.setBounds (PADDING * 3 + boxWidth + sliderWidth, PADDING, boxWidth, getHeight() - PADDING * 2);
    }

private:
    int mIndex;
    String name = "mod";
    AudioProcessorValueTreeState& mParameters;
    ComboBox sources, destinations;
    std::unique_ptr<ComboBoxAttachment> sourcesAttachment, destinationAttachment;
    Slider amountSlider;
    std::unique_ptr<SliderAttachment> amountAttachment;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ModulationModule)
};
