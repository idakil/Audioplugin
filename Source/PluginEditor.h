/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class AudiopluginAudioProcessorEditor  : public AudioProcessorEditor
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
    AudioProcessorValueTreeState& valueTreeState;
    FlexBox flexBox;

    // Delay
    Label delayLenghtLabel;
    Slider delayLenghtSlider;
    Label delayModAmountLabel;
    Slider delayModAmountSlider;
    Label delayLfoSpeedLabel;
    Slider delayLfoSpeedSlider;
    Label delayFeedbackLabel;
    Slider delayFeedbackSlider;
    Label delaywetDryMixLabel;
    Slider delaywetDryMixSlider;
    
    std::unique_ptr<SliderAttachment> delayLenghtAttachment;
    std::unique_ptr<SliderAttachment> delayModAmountAttachment;
    std::unique_ptr<SliderAttachment> delayLfoSpeedAttachment;
    std::unique_ptr<SliderAttachment> delayFeedbackAttachment;
    std::unique_ptr<SliderAttachment> delaywetDryMixAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudiopluginAudioProcessorEditor)
};
