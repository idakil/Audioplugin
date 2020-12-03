
#include "PluginEditor.h"

DistortionComponent::DistortionComponent(DistortionProcessor& processor)
    : thresholdSlider(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow)
    , ratioSlider(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow)
    , attackSlider(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow)
    , releaseSlider(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow)
    , saturationSlider(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow)
    , thresholdAttachment(*processor.thresholdParam, thresholdSlider)
    , ratioAttachment(*processor.ratioParam, ratioSlider)
    , attackAttachment(*processor.attackParam, attackSlider)
    , releaseAttachment(*processor.releaseParam, releaseSlider)
    , saturationAttachment(*processor.saturationParam, saturationSlider)
{

    const juce::FlexItem::Margin margin = juce::FlexItem::Margin(25.0f, 0.0f, 20.0f, 0.0f);

    addAndMakeVisible(thresholdSlider);
    addAndMakeVisible(ratioSlider);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(releaseSlider);
    addAndMakeVisible(saturationSlider);

    distortionFlexBox.items.add(juce::FlexItem(thresholdSlider).withFlex(1).withMargin(margin));
    distortionFlexBox.items.add(juce::FlexItem(ratioSlider).withFlex(1).withMargin(margin));
    distortionFlexBox.items.add(juce::FlexItem(attackSlider).withFlex(1).withMargin(margin));
    distortionFlexBox.items.add(juce::FlexItem(releaseSlider).withFlex(1).withMargin(margin));
    distortionFlexBox.items.add(juce::FlexItem(saturationSlider).withFlex(1).withMargin(margin));
}

void DistortionComponent::paint(juce::Graphics& g)
{


    g.setColour(juce::Colours::beige);

    int labelW = 100;
    g.drawText("Threshold", thresholdSlider.getX() + thresholdSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("Ratio??", ratioSlider.getX() + ratioSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("Attack", attackSlider.getX() + attackSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("Release", releaseSlider.getX() + releaseSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("Saturation", saturationSlider.getX() + saturationSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
}

void DistortionComponent::resized()
{
    auto bounds = getLocalBounds();

    distortionFlexBox.performLayout(bounds);
}

ChorusComponent::ChorusComponent(ChorusProcessor& processor)
    : chorusLenghtSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , chorusModulationSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , chorusLfoSpeedSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , chorusWetDryMixSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , chorusLenghtAttachment(*processor.lenghtParam, chorusLenghtSlider)
    , chorusModulationAttachment(*processor.modAmountParam, chorusModulationSlider)
    , chorusLfoAttachment(*processor.lfoSpeedParam, chorusLfoSpeedSlider)
    , chorusWetDryMixAttachment(*processor.wetDryMixParam, chorusWetDryMixSlider)
{
    const juce::FlexItem::Margin margin = juce::FlexItem::Margin(25.0f, 0.0f, 15.0f, 0.0f);

    addAndMakeVisible(chorusLenghtSlider);
    addAndMakeVisible(chorusModulationSlider);
    addAndMakeVisible(chorusLfoSpeedSlider);
    addAndMakeVisible(chorusWetDryMixSlider);

    chorusFlexBox.items.add(juce::FlexItem(chorusLenghtSlider).withFlex(1).withMargin(margin));
    chorusFlexBox.items.add(juce::FlexItem(chorusModulationSlider).withFlex(1).withMargin(margin));
    chorusFlexBox.items.add(juce::FlexItem(chorusLfoSpeedSlider).withFlex(1).withMargin(margin));
    chorusFlexBox.items.add(juce::FlexItem(chorusWetDryMixSlider).withFlex(1).withMargin(margin));
}

void ChorusComponent::paint(juce::Graphics& g)
{


    g.setColour(juce::Colours::beige);

    int labelW = 100;
    g.drawText("Delay lenght", chorusLenghtSlider.getX() + chorusLenghtSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("Modulation", chorusModulationSlider.getX() + chorusModulationSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("LFO speed", chorusLfoSpeedSlider.getX() + chorusLfoSpeedSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("Wet / Dry", chorusWetDryMixSlider.getX() + chorusWetDryMixSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
}

void ChorusComponent::resized()
{
    auto bounds = getLocalBounds();

    chorusFlexBox.performLayout(bounds);
}

DelayComponent::DelayComponent(DelayProcessor& processor)
    : delayLenghtSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , delayFeedbackSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , delayWetDryMixSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , delayLenghtAttachment(*processor.lenghtParam, delayLenghtSlider)
    , delayFeedbackAttachment(*processor.feedbackParam, delayFeedbackSlider)
    , delayWetDryMixAttachment(*processor.wetDryMixParam, delayWetDryMixSlider)
{

    const juce::FlexItem::Margin margin = juce::FlexItem::Margin(25.0f, 0.0f, 15.0f, 0.0f);
    
    addAndMakeVisible(delayLenghtSlider);
    addAndMakeVisible(delayFeedbackSlider);
    addAndMakeVisible(delayWetDryMixSlider);

    delayLenghtSlider.setTextValueSuffix(" ms");
    delayFeedbackSlider.setTextValueSuffix(" ms");

    delayFlexBox.items.add(juce::FlexItem(delayLenghtSlider).withFlex(1).withMargin(margin));
    delayFlexBox.items.add(juce::FlexItem(delayFeedbackSlider).withFlex(1).withMargin(margin));
    delayFlexBox.items.add(juce::FlexItem(delayWetDryMixSlider).withFlex(1).withMargin(margin));
}

void DelayComponent::paint(juce::Graphics& g)
{

    g.setColour(juce::Colours::beige);
    int labelW = 100;
    g.drawText("Delay lenght", delayLenghtSlider.getX() + delayLenghtSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("Feedback", delayFeedbackSlider.getX() + delayFeedbackSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("Dry / Wet", delayWetDryMixSlider.getX() + delayWetDryMixSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
}

void DelayComponent::resized()
{
    auto bounds = getLocalBounds();

    delayFlexBox.performLayout(bounds);
}

CompressorComponent::CompressorComponent(Compressor& comp)
    : threshSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , slopeSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , attackSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , releaseSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)

    , threshAttachment(*comp.threshParam, threshSlider)
    , slopeAttachment(*comp.ratioParam, slopeSlider)
    , attackAttachment(*comp.attackParam, attackSlider)
    , releaseAttachment(*comp.releaseParam, releaseSlider)
{

    addAndMakeVisible(threshSlider);
    addAndMakeVisible(slopeSlider);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(releaseSlider);

    compressorFlexBox.items.add(juce::FlexItem(threshSlider).withFlex(1));
    compressorFlexBox.items.add(juce::FlexItem(slopeSlider).withFlex(1));
    compressorFlexBox.items.add(juce::FlexItem(attackSlider).withFlex(1));
    compressorFlexBox.items.add(juce::FlexItem(releaseSlider).withFlex(1));
}

void CompressorComponent::paint(juce::Graphics& g) {


    g.setColour(juce::Colours::beige);

    int labelW = 100;
    g.drawText("Thresh", threshSlider.getX() + threshSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("Ratio", slopeSlider.getX() + slopeSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("Attack", attackSlider.getX() + attackSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("Release", releaseSlider.getX() + releaseSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
};
void CompressorComponent::resized() {
    int w = 100;
    int y = 45;
    auto bounds = getLocalBounds();
    threshSlider.setBounds(getWidth() / 5 - w/3 , y, w, getHeight() - sliderHeightMargin);
    slopeSlider.setBounds(2 * getWidth() / 5 - w /3 , y, w, getHeight() - sliderHeightMargin);
    attackSlider.setBounds(3 * getWidth() / 5 - w / 3 , y, w, getHeight() - sliderHeightMargin);
    releaseSlider.setBounds(4 * getWidth() / 5 - w / 3 , y, w, getHeight() - sliderHeightMargin);

  
   //compressorFlexBox.performLayout(bounds);
};
//==============================================================================

BitCrusherComponent::BitCrusherComponent(BitCrusher& bitCrusher)
    : bitReduxSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , rateReduxSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , noiseSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
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

void BitCrusherComponent::paint(juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)

    g.setColour(juce::Colours::beige);
    int labelW = 100;
    g.drawText("Noise", noiseSlider.getX() + noiseSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("BitRedux", bitReduxSlider.getX() + bitReduxSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
    g.drawText("RateRedux", rateReduxSlider.getX() + rateReduxSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, juce::Justification::centred);
}

void BitCrusherComponent::resized()
{
    int w = 60;
    int y = 50;

    noiseSlider.setBounds(getWidth() / 4 - w / 2, y, w, getHeight() - sliderHeightMargin);
    bitReduxSlider.setBounds(2 * getWidth() / 4 - w / 2, y, w, getHeight() - sliderHeightMargin);
    rateReduxSlider.setBounds(3 * getWidth() / 4 - w / 2, y, w, getHeight() - sliderHeightMargin);
    auto bounds = getLocalBounds();

    //bitCrusherFlexBox.performLayout(bounds);

}
//==============================================================================

EqBandComponent::EqBandComponent(Equaliser& eq)
    : freqSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , qualSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , gainSlider(juce::Slider::LinearVertical, juce::Slider::TextBoxBelow)
    , freqAttachment(*eq.freqParam, freqSlider)
    , qualAttachment(*eq.qualParam, qualSlider)
    , gainAttachment(*eq.gainParam, gainSlider)
{

    freqSlider.setSkewFactorFromMidPoint(1000);
    header.setButtonText(eq.typeParam->getCurrentChoiceName());
    addAndMakeVisible(header);
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(qualSlider);
    addAndMakeVisible(gainSlider);
    eqFlexBox.items.add(juce::FlexItem(header).withFlex(1));
    eqFlexBox.items.add(juce::FlexItem(freqSlider).withFlex(1));
    eqFlexBox.items.add(juce::FlexItem(qualSlider).withFlex(1));
    eqFlexBox.items.add(juce::FlexItem(gainSlider).withFlex(1));

    eqFlexBox.flexWrap = juce::FlexBox::Wrap::wrap;
    eqFlexBox.flexDirection = juce::FlexBox::Direction::row;
    eqFlexBox.justifyContent = juce::FlexBox::JustifyContent::center;
}

void EqBandComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::beige);
    int labelW = 100;
    g.drawText("Freq", freqSlider.getX() + freqSlider.getWidth() / 2 - labelW / 2, 40, labelW, 20, juce::Justification::centred);
    g.drawText("qual", qualSlider.getX() + qualSlider.getWidth() / 2 - labelW / 2, 40, labelW, 20, juce::Justification::centred);
    g.drawText("gain", gainSlider.getX() + gainSlider.getWidth() / 2 - labelW / 2, 40, labelW, 20, juce::Justification::centred);
}

void EqBandComponent::resized()
{
    int w = 60;
    int y = 50;
    auto bounds = getLocalBounds();
    freqSlider.setBounds(getWidth() / 4 - w / 2, y, w, getHeight() - sliderHeightMargin);
    qualSlider.setBounds(2 * getWidth() / 4 - w / 2, y, w, getHeight() - sliderHeightMargin);
    gainSlider.setBounds(3 * getWidth() / 4 - w / 2, y, w, getHeight() - sliderHeightMargin);

    auto headerFooterHeight = 25;
    header.setBounds(0, 0, header.getParentWidth(), headerFooterHeight);

    //eqFlexBox.performLayout(bounds);
}

//==============================================================================
AudiopluginAudioProcessorEditor::AudiopluginAudioProcessorEditor (AudiopluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), 
    audioProcessor (p), 
    valueTreeState(vts), 
    container(p),
    main(p)
{
    setResizable(true, true);
    tabbedComponent.reset(new juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible(tabbedComponent.get());
    tabbedComponent->setTabBarDepth(35);
    tabbedComponent->addTab(TRANS("Perkelidy"), juce::Colour(0xffc483f2), &main, false);
    tabbedComponent->addTab(TRANS("Advanced"), juce::Colour(0xffc483f2), &container, false);
    tabbedComponent->setCurrentTabIndex(0);
    //tabbedComponent->setSize(windowWidth, windowHeight);

    setSize(windowWidth, windowHeight);
}

AudiopluginAudioProcessorEditor::~AudiopluginAudioProcessorEditor()
{
}

//==============================================================================
void AudiopluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xffb140f7));
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colour(0xff6500a3));
    getLookAndFeel().setColour(juce::Slider::backgroundColourId, juce::Colours::beige);
    getLookAndFeel().setColour(juce::Slider::trackColourId, juce::Colour(0xff6500a3));
}

void AudiopluginAudioProcessorEditor::resized()
{
    tabbedComponent->setBounds(0, 0, getWidth(), getHeight());
}
