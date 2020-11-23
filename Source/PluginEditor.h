/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;

//==============================================================================
/**
*/
struct EqBandComponent : public Component
{
    EqBandComponent(Equaliser& eq);
    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider freqSlider;
    Slider qualSlider;
    Slider gainSlider;
    SliderParameterAttachment freqAttachment;
    SliderParameterAttachment qualAttachment;
    SliderParameterAttachment gainAttachment;
    FlexBox eqFlexBox;
};

struct CompressorComponent : public Component {

    CompressorComponent(Compressor& comp);
    ~CompressorComponent() {};

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider threshSlider, slopeSlider, kneeSlider, attackSlider, releaseSlider;
    Label threshLabel, slopeLabel, kneeLabel, attackLabel, releaseLabel;
    SliderParameterAttachment threshAttachment, slopeAttachment, kneeAttachment, attackAttachment, releaseAttachment;
    FlexBox compressorFlexBox;
};

struct BitCrusherComponent : public Component {
    BitCrusherComponent(BitCrusher& bitCrusher);
    ~BitCrusherComponent() {};

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider bitReduxSlider, rateReduxSlider, noiseSlider;
    SliderParameterAttachment bitReduxAttachment, rateReduxAttachment, noiseAttachment;
    FlexBox bitCrusherFlexBox;
};

class AudiopluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    enum
    {
        marginTop = 80,
        gainLabelWidth = 80,
        gainSliderWidth = 300,
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
    CompressorComponent compressorKnobs;
    BitCrusherComponent bitCrusherKnobs;
    EqBandComponent eqKnobs0;
    EqBandComponent eqKnobs1;

    juce::AudioProcessorValueTreeState& valueTreeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudiopluginAudioProcessorEditor)
};
