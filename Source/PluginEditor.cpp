/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
//==============================================================================
AudiopluginAudioProcessorEditor::AudiopluginAudioProcessorEditor (AudiopluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts)
{
    addSlider("thresh", "Threshold", threshSlider, threshLabel, threshAttachment);
    addSlider("ratio", "Ratio", slopeSlider, slopeLabel, slopeAttachment);
    addSlider("knee", "Knee", kneeSlider, kneeLabel, kneeAttachment);
    addSlider("attack", "Attack", attackSlider, attackLabel, attackAttachment);
    addSlider("release", "Release", releaseSlider, releaseLabel, releaseAttachment);
 
    //addAndMakeVisible(bandKnobs0);
    //addAndMakeVisible(bandKnobs1);
    //flexBox.items.add(juce::FlexItem(bandKnobs0).withMinHeight(10.0f).withFlex(1));
    //flexBox.items.add(juce::FlexItem(bandKnobs1).withMinHeight(10.0f).withFlex(1));

    // Flexbox Style
    flexBox.flexDirection = juce::FlexBox::Direction::column;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
    setSize(gainSliderWidth + gainLabelWidth, juce::jmax(600, marginTop * 2));
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
    auto bounds = getLocalBounds();
    int h = bounds.getHeight() / 2;
    //bandKnobs0.setBounds(bounds.removeFromTop(h));
    //bandKnobs1.setBounds(bounds);
    threshSlider.setBounds(100, 0, 200, 50);
    slopeSlider.setBounds(100, 50, 200, 50);
    kneeSlider.setBounds(100, 100, 200, 50);
    attackSlider.setBounds(100, 150, 200, 50);
    releaseSlider.setBounds(100, 200, 200, 50);
}

void AudiopluginAudioProcessorEditor::addSlider(String name, String labelText, Slider& slider, Label& label, std::unique_ptr<SliderAttachment>& attachment) {
    addAndMakeVisible(slider);
    attachment.reset(new SliderAttachment(valueTreeState, name, slider));
    label.setText(labelText, dontSendNotification);
    label.attachToComponent(&slider, true);
    addAndMakeVisible(label);
    flexBox.items.add(juce::FlexItem(label).withMinHeight(10.0f).withFlex(1));
    flexBox.items.add(juce::FlexItem(slider).withMinHeight(10.0f).withFlex(1));
}