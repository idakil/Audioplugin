
#include "PluginEditor.h"

CompressorComponent::CompressorComponent(Compressor& comp)
    : threshSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , slopeSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , kneeSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , attackSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , releaseSlider(Slider::LinearVertical, Slider::TextBoxBelow)

    , threshAttachment(*comp.threshParam, threshSlider)
    , slopeAttachment(*comp.slopeParam, slopeSlider)
    , kneeAttachment(*comp.kneeParam, kneeSlider)
    , attackAttachment(*comp.attackParam, attackSlider)
    , releaseAttachment(*comp.releaseParam, releaseSlider)
{
    addAndMakeVisible(threshSlider);
    addAndMakeVisible(slopeSlider);
    addAndMakeVisible(kneeSlider);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(releaseSlider);

    compressorFlexBox.items.add(juce::FlexItem(threshSlider).withFlex(1));
    compressorFlexBox.items.add(juce::FlexItem(slopeSlider).withFlex(1));
    compressorFlexBox.items.add(juce::FlexItem(kneeSlider).withFlex(1));
    compressorFlexBox.items.add(juce::FlexItem(attackSlider).withFlex(1));
    compressorFlexBox.items.add(juce::FlexItem(releaseSlider).withFlex(1));

}

void CompressorComponent::paint(juce::Graphics& g) {
    g.fillAll(Colours::midnightblue.withMultipliedBrightness(.4));

    g.setColour(Colours::beige);

    int labelW = 100;
    g.drawText("Thresh", threshSlider.getX() + threshSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);
    g.drawText("Slope", slopeSlider.getX() + slopeSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);
    g.drawText("Knee", kneeSlider.getX() + kneeSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);
    g.drawText("Attack", attackSlider.getX() + attackSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);
    g.drawText("Release", releaseSlider.getX() + releaseSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);

};
void CompressorComponent::resized() {
    int margin = 10;
    int w = 60;
    int y = 50;
    auto bounds = getLocalBounds();
    threshSlider.setBounds(getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    slopeSlider.setBounds(2 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    kneeSlider.setBounds(3 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    attackSlider.setBounds(3 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    releaseSlider.setBounds(3 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    compressorFlexBox.performLayout(bounds);
};

BitCrusherComponent::BitCrusherComponent(BitCrusher& bitCrusher)
    : bitReduxSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , rateReduxSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , noiseSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , bitReduxAttachment(*bitCrusher.bitReduxParam, bitReduxSlider)
    , rateReduxAttachment(*bitCrusher.rateReduxParam, rateReduxSlider)
    , noiseAttachment(*bitCrusher.noiseParam, noiseSlider)
{
    addAndMakeVisible(rateReduxSlider);
    addAndMakeVisible(bitReduxSlider);
    addAndMakeVisible(noiseSlider);

    bitCrusherFlexBox.items.add(juce::FlexItem(rateReduxSlider).withFlex(1));
    bitCrusherFlexBox.items.add(juce::FlexItem(bitReduxSlider).withFlex(1));
    bitCrusherFlexBox.items.add(juce::FlexItem(noiseSlider).withFlex(1));

}

void BitCrusherComponent::paint(Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::midnightblue.withMultipliedBrightness(.4));
    g.setColour(Colours::beige);
    int labelW = 100;
    g.drawText("Noise", noiseSlider.getX() + noiseSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);
    g.drawText("Bit", bitReduxSlider.getX() + bitReduxSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);
    g.drawText("Rate", rateReduxSlider.getX() + rateReduxSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);
}

void BitCrusherComponent::resized()
{

    int margin = 10;
    int w = 60;
    int y = 50;
    noiseSlider.setBounds(getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    bitReduxSlider.setBounds(2 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    rateReduxSlider.setBounds(3 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    auto bounds = getLocalBounds();

    bitCrusherFlexBox.performLayout(bounds);

}

EqBandComponent::EqBandComponent(Equaliser& eq)
    : freqSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , qualSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , gainSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , freqAttachment(*eq.freqParam, freqSlider)
    , qualAttachment(*eq.qualParam, qualSlider)
    , gainAttachment(*eq.gainParam, gainSlider)
{
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(qualSlider);
    addAndMakeVisible(gainSlider);

    eqFlexBox.items.add(juce::FlexItem(freqSlider).withFlex(1));
    eqFlexBox.items.add(juce::FlexItem(qualSlider).withFlex(1));
    eqFlexBox.items.add(juce::FlexItem(gainSlider).withFlex(1));

}

void EqBandComponent::paint(juce::Graphics& g)
{
    g.fillAll(Colours::midnightblue.withMultipliedBrightness(.4));
    g.setColour(Colours::beige);
    int labelW = 100;
    g.drawText("Freq", freqSlider.getX() + freqSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);
    g.drawText("qual", qualSlider.getX() + qualSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);
    g.drawText("gain", gainSlider.getX() + gainSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);
}

void EqBandComponent::resized()
{
    int margin = 10;
    int w = 60;
    int y = 50;
    freqSlider.setBounds(getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    qualSlider.setBounds(2 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    gainSlider.setBounds(3 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    auto bounds = getLocalBounds();

    eqFlexBox.performLayout(bounds);
}

//==============================================================================
AudiopluginAudioProcessorEditor::AudiopluginAudioProcessorEditor (AudiopluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p), valueTreeState(vts), compressorKnobs(p.compressor), eqKnobs0(p.eq0), eqKnobs1(p.eq1), bitCrusherKnobs(p.bitCrusher)
{

    addAndMakeVisible(compressorKnobs);
    flexBox.items.add(juce::FlexItem(compressorKnobs).withMinHeight(10.0f).withFlex(1));

    addAndMakeVisible(eqKnobs0);
    flexBox.items.add(juce::FlexItem(eqKnobs0).withMinHeight(10.0f).withFlex(1));

    addAndMakeVisible(eqKnobs1);
    flexBox.items.add(juce::FlexItem(eqKnobs1).withMinHeight(10.0f).withFlex(1));

    addAndMakeVisible(bitCrusherKnobs);
    flexBox.items.add(juce::FlexItem(bitCrusherKnobs).withMinHeight(10.0f).withFlex(1));


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
