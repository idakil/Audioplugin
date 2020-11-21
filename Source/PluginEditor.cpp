/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginEditor.h"
//==============================================================================
AudiopluginAudioProcessorEditor::AudiopluginAudioProcessorEditor (AudiopluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts), eq(p), comp(p), bitCrusher(p)
{
    /*addSlider("thresh", "Threshold", threshSlider, threshLabel, threshAttachment);
    addSlider("ratio", "Ratio", slopeSlider, slopeLabel, slopeAttachment);
    addSlider("knee", "Knee", kneeSlider, kneeLabel, kneeAttachment);
    addSlider("attack", "Attack", attackSlider, attackLabel, attackAttachment);
    addSlider("release", "Release", releaseSlider, releaseLabel, releaseAttachment);*/
 
    //addAndMakeVisible(bandKnobs0);
    //addAndMakeVisible(bandKnobs1);


    addAndMakeVisible(comp);
    flexBox.items.add(juce::FlexItem(comp).withMinHeight(10.0f).withFlex(1));

    addAndMakeVisible(bitCrusher);
    flexBox.items.add(juce::FlexItem(bitCrusher).withMinHeight(10.0f).withFlex(1));


    addAndMakeVisible(eq);
    flexBox.items.add(juce::FlexItem(eq).withMinHeight(10.0f).withFlex(1));



    // Flexbox Style
    flexBox.flexDirection = juce::FlexBox::Direction::column;
    flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
    setSize(800,800);
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

void AudiopluginAudioProcessorEditor::addSlider(String name, String labelText, Slider& slider, Label& label, std::unique_ptr<SliderAttachment>& attachment) {
    addAndMakeVisible(slider);
    attachment.reset(new SliderAttachment(valueTreeState, name, slider));
    label.setText(labelText, dontSendNotification);
    label.attachToComponent(&slider, true);
    addAndMakeVisible(label);
    flexBox.items.add(juce::FlexItem(label).withMinHeight(10.0f).withFlex(1));
    flexBox.items.add(juce::FlexItem(slider).withMinHeight(10.0f).withFlex(1));
}