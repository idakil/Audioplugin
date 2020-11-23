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

struct DelayComponent : public Component {
    DelayComponent(DelayProcessor& processor);

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider delayLenghtSlider;
    Slider delayModulationSlider;
    Slider delayLfoSpeedSlider;
    Slider delayFeedbackSlider;
    Slider delayWetDryMixSlider;

    SliderParameterAttachment delayLenghtAttachment;
    SliderParameterAttachment delayModulationAttachment;
    SliderParameterAttachment delayLfoAttachment;
    SliderParameterAttachment delayFeedbackAttachment;
    SliderParameterAttachment delayWetDryMixAttachment;

    FlexBox delayFlexBox;
};

struct ChorusComponent : public Component {
    
    ChorusComponent(ChorusProcessor& processor);

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider chorusLenghtSlider;
    Slider chorusModulationSlider;
    Slider chorusLfoSpeedSlider;
    Slider chorusFeedbackSlider;
    Slider chorusWetDryMixSlider;

    SliderParameterAttachment chorusLenghtAttachment;
    SliderParameterAttachment chorusModulationAttachment;
    SliderParameterAttachment chorusLfoAttachment;
    SliderParameterAttachment chorusFeedbackAttachment;
    SliderParameterAttachment chorusWetDryMixAttachment;

    FlexBox chorusFlexBox;
};
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
    FlexBox flexBox;

    ChorusComponent chorusKnobs;
    DelayComponent delayKnobs;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudiopluginAudioProcessorEditor)
};
