/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AudiopluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    enum
    {
        marginTop = 80,
        gainLabelWidth = 80,
        gainSliderWidth = 300
    };

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

    AudiopluginAudioProcessorEditor(AudiopluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~AudiopluginAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    AudiopluginAudioProcessor& audioProcessor;
    juce::AudioProcessorValueTreeState& valueTreeState;

    // Gain
    juce::Label gainLabel;
    juce::Slider gainSlider;
    std::unique_ptr<SliderAttachment> gainAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudiopluginAudioProcessorEditor)
};
