/*
  ==============================================================================

    BitCrusher.h
    Created: 13 Nov 2020 12:41:10pm
    Author:  idakil

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class BitCrusher : public AudioProcessorEditor, public Slider::Listener
{
public:
    BitCrusher(BitCrusherProcessor&);
    ~BitCrusher();

    //==============================================================================
    void paint(Graphics&) override;
    void resized() override;

    Slider bitRedux, rateRedux, noise;

private:

    void sliderValueChanged(Slider* slider) override;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BitCrusherProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitCrusher)
};