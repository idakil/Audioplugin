/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AudiopluginAudioProcessorEditor::AudiopluginAudioProcessorEditor (AudiopluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
    // Lenght
    addAndMakeVisible(delayLenghtLabel);
    delayLenghtLabel.setText("Delay Length", juce::dontSendNotification);
    flexBox.items.add(juce::FlexItem(delayLenghtLabel).withMinHeight(10.0f).withFlex(1));

    addAndMakeVisible(delayLenghtSlider);
    delayLenghtSlider.setSliderStyle(Slider::LinearBar);
    delayLenghtAttachment.reset(new SliderAttachment(valueTreeState, "delay_lenght", delayLenghtSlider));
    flexBox.items.add(juce::FlexItem(delayLenghtSlider).withMinHeight(33.0f).withFlex(1));

    // Mod Amount
    addAndMakeVisible(delayModAmountLabel);
    delayModAmountLabel.setText("Delay Modulation (s)", juce::dontSendNotification);
    flexBox.items.add(juce::FlexItem(delayModAmountLabel).withMinHeight(10.0f).withFlex(1));

    delayModAmountSlider.setSliderStyle(Slider::LinearBar);
    addAndMakeVisible(delayModAmountSlider);
    delayModAmountAttachment.reset(new SliderAttachment(valueTreeState, "delay_modamount", delayModAmountSlider));
    flexBox.items.add(juce::FlexItem(delayModAmountSlider).withMinHeight(33.0f).withFlex(1));

    // LFO Speed
    addAndMakeVisible(delayLfoSpeedLabel);
    delayLfoSpeedLabel.setText("Delay LFO Speed (ms)", juce::dontSendNotification);
    flexBox.items.add(juce::FlexItem(delayLfoSpeedLabel).withMinHeight(10.0f).withFlex(1));

    delayLfoSpeedSlider.setSliderStyle(Slider::LinearBar);
    addAndMakeVisible(delayLfoSpeedSlider);
    delayLfoSpeedAttachment.reset(new SliderAttachment(valueTreeState, "delay_lfospeed", delayLfoSpeedSlider));
    flexBox.items.add(juce::FlexItem(delayLfoSpeedSlider).withMinHeight(33.0f).withFlex(1));

    // Feedback
    addAndMakeVisible(delayFeedbackLabel);
    delayFeedbackLabel.setText("Feedback", juce::dontSendNotification);
    flexBox.items.add(juce::FlexItem(delayFeedbackLabel).withMinHeight(10.0f).withFlex(1));

    delayFeedbackSlider.setSliderStyle(Slider::LinearBar);
    addAndMakeVisible(delayFeedbackSlider);
    delayFeedbackAttachment.reset(new SliderAttachment(valueTreeState, "delay_feedback", delayFeedbackSlider));
    flexBox.items.add(juce::FlexItem(delayFeedbackSlider).withMinHeight(33.0f).withFlex(1));

    // Wet / Dry
    addAndMakeVisible(delaywetDryMixLabel);
    delaywetDryMixLabel.setText("Wet / Dry", juce::dontSendNotification);
    flexBox.items.add(juce::FlexItem(delaywetDryMixLabel).withMinHeight(10.0f).withFlex(1));

    delaywetDryMixSlider.setSliderStyle(Slider::LinearBar);
    addAndMakeVisible(delaywetDryMixSlider);
    delaywetDryMixAttachment.reset(new SliderAttachment(valueTreeState, "delay_wetdry", delaywetDryMixSlider));
    flexBox.items.add(juce::FlexItem(delaywetDryMixSlider).withMinHeight(33.0f).withFlex(1));

    // Flexbox Style
    flexBox.flexDirection = juce::FlexBox::Direction::column;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::center;

    setSize (gainSliderWidth + gainLabelWidth, juce::jmax(600, marginTop * 2));
}

AudiopluginAudioProcessorEditor::~AudiopluginAudioProcessorEditor()
{
}

//==============================================================================
void AudiopluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AudiopluginAudioProcessorEditor::resized()
{
    flexBox.performLayout(getLocalBounds().toFloat());
}
