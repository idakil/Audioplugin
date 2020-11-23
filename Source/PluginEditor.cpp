/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

DelayComponent::DelayComponent(DelayProcessor& processor)
    : delayLenghtSlider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow)
    , delayModulationSlider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow)
    , delayLfoSpeedSlider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow)
    , delayFeedbackSlider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow)
    , delayWetDryMixSlider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow)
    , delayLenghtAttachment(*processor.lenghtParam, delayLenghtSlider)
    , delayModulationAttachment(*processor.modAmountParam, delayModulationSlider)
    , delayLfoAttachment(*processor.lfoSpeedParam, delayLfoSpeedSlider)
    , delayFeedbackAttachment(*processor.feedbackParam, delayFeedbackSlider)
    , delayWetDryMixAttachment(*processor.wetDryMixParam, delayWetDryMixSlider)
{
    addAndMakeVisible(delayLenghtSlider);
    addAndMakeVisible(delayModulationSlider);
    addAndMakeVisible(delayLfoSpeedSlider);
    addAndMakeVisible(delayFeedbackSlider);
    addAndMakeVisible(delayWetDryMixSlider);

    delayFlexBox.items.add(juce::FlexItem(delayLenghtSlider).withFlex(1));
    delayFlexBox.items.add(juce::FlexItem(delayModulationSlider).withFlex(1));
    delayFlexBox.items.add(juce::FlexItem(delayLfoSpeedSlider).withFlex(1));
    delayFlexBox.items.add(juce::FlexItem(delayFeedbackSlider).withFlex(1));
    delayFlexBox.items.add(juce::FlexItem(delayWetDryMixSlider).withFlex(1));
}


void DelayComponent::paint(juce::Graphics& g)
{
}

void DelayComponent::resized()
{
    auto bounds = getLocalBounds();

    delayFlexBox.performLayout(bounds);
}


ChorusComponent::ChorusComponent(ChorusProcessor& processor)
    : chorusLenghtSlider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow)
    , chorusModulationSlider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow)
    , chorusLfoSpeedSlider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow)
    , chorusFeedbackSlider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow)
    , chorusWetDryMixSlider(Slider::RotaryVerticalDrag, Slider::TextBoxBelow)
    , chorusLenghtAttachment(*processor.lenghtParam, chorusLenghtSlider)
    , chorusModulationAttachment(*processor.modAmountParam , chorusModulationSlider)
    , chorusLfoAttachment(*processor.lfoSpeedParam, chorusLfoSpeedSlider)
    , chorusFeedbackAttachment(*processor.feedbackParam, chorusFeedbackSlider)
    , chorusWetDryMixAttachment(*processor.wetDryMixParam, chorusWetDryMixSlider)
{
    addAndMakeVisible(chorusLenghtSlider);
    addAndMakeVisible(chorusModulationSlider);
    addAndMakeVisible(chorusLfoSpeedSlider);
    addAndMakeVisible(chorusFeedbackSlider);
    addAndMakeVisible(chorusWetDryMixSlider);

    chorusFlexBox.items.add(juce::FlexItem(chorusLenghtSlider).withFlex(1));
    chorusFlexBox.items.add(juce::FlexItem(chorusModulationSlider).withFlex(1));
    chorusFlexBox.items.add(juce::FlexItem(chorusLfoSpeedSlider).withFlex(1));
    chorusFlexBox.items.add(juce::FlexItem(chorusFeedbackSlider).withFlex(1));
    chorusFlexBox.items.add(juce::FlexItem(chorusWetDryMixSlider).withFlex(1));
}

void ChorusComponent::paint(juce::Graphics& g)
{
}

void ChorusComponent::resized()
{
    auto bounds = getLocalBounds();
    
    chorusFlexBox.performLayout(bounds);
}

//==============================================================================
AudiopluginAudioProcessorEditor::AudiopluginAudioProcessorEditor (AudiopluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), chorusKnobs(p.chorus), delayKnobs(p.delay)
{
    addAndMakeVisible(chorusKnobs);
    flexBox.items.add(juce::FlexItem(chorusKnobs).withFlex(1));

    addAndMakeVisible(delayKnobs);
    flexBox.items.add(juce::FlexItem(delayKnobs).withFlex(1));
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
