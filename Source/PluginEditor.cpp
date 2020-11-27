
#include "PluginEditor.h"

CompressorComponent::CompressorComponent(Compressor& comp)
    : threshSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , slopeSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , attackSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , releaseSlider(Slider::LinearVertical, Slider::TextBoxBelow)

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
    g.fillAll(Colours::midnightblue.withMultipliedBrightness(.4));

    g.setColour(Colours::beige);

    int labelW = 100;
    g.drawText("Thresh", threshSlider.getX() + threshSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, Justification::centred);
    g.drawText("Slope", slopeSlider.getX() + slopeSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, Justification::centred);
    g.drawText("Attack", attackSlider.getX() + attackSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, Justification::centred);
    g.drawText("Release", releaseSlider.getX() + releaseSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, Justification::centred);
};
void CompressorComponent::resized() {
    int w = 100;
    int y = 80;
    auto bounds = getLocalBounds();
    threshSlider.setBounds(getWidth() / 5 - w/3 , y, w, getHeight() - sliderHeightMargin);
    slopeSlider.setBounds(2 * getWidth() / 5 - w /3 , y, w, getHeight() - sliderHeightMargin);
    attackSlider.setBounds(3 * getWidth() / 5 - w / 3 , y, w, getHeight() - sliderHeightMargin);
    releaseSlider.setBounds(4 * getWidth() / 5 - w / 3 , y, w, getHeight() - sliderHeightMargin);

  
   //compressorFlexBox.performLayout(bounds);
};
//==============================================================================

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
    g.drawText("Noise", noiseSlider.getX() + noiseSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, Justification::centred);
    g.drawText("BitRedux", bitReduxSlider.getX() + bitReduxSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, Justification::centred);
    g.drawText("RateRedux", rateReduxSlider.getX() + rateReduxSlider.getWidth() / 2 - labelW / 2, labelPosition, labelW, 20, Justification::centred);
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
    : freqSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , qualSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , gainSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , freqAttachment(*eq.freqParam, freqSlider)
    , qualAttachment(*eq.qualParam, qualSlider)
    , gainAttachment(*eq.gainParam, gainSlider)
{

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
    g.fillAll(Colours::midnightblue.withMultipliedBrightness(.4));
    g.setColour(Colours::beige);
    int labelW = 100;
    g.drawText("Freq", freqSlider.getX() + freqSlider.getWidth() / 2 - labelW / 2, 40, labelW, 20, Justification::centred);
    g.drawText("qual", qualSlider.getX() + qualSlider.getWidth() / 2 - labelW / 2, 40, labelW, 20, Justification::centred);
    g.drawText("gain", gainSlider.getX() + gainSlider.getWidth() / 2 - labelW / 2, 40, labelW, 20, Justification::centred);
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
    header.setBounds(0, 0, windowWidth, headerFooterHeight);

    //eqFlexBox.performLayout(bounds);
}


//==============================================================================
AudiopluginAudioProcessorEditor::AudiopluginAudioProcessorEditor (AudiopluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), 
    audioProcessor (p), 
    valueTreeState(vts), 
    container(p),
    main(p)
    /*compressorKnobs0(p.compressor0), 
    compressorKnobs1(p.compressor1),
    eqKnobs0(p.eq0), 
    eqKnobs1(p.eq1), 
    bitCrusherKnobs(p.bitCrusher)*/
{
    tabbedComponent.reset(new juce::TabbedComponent(juce::TabbedButtonBar::TabsAtTop));
    addAndMakeVisible(tabbedComponent.get());
    tabbedComponent->setTabBarDepth(35);
    tabbedComponent->addTab(TRANS("Perkelidy"), juce::Colour(0xffc483f2), new MainView(p), false);
    tabbedComponent->addTab(TRANS("Advanced"), juce::Colour(0xffc483f2), new EffectComponentContainer(p) , false);
    tabbedComponent->setCurrentTabIndex(0);

    setSize(windowWidth, windowHeight);
}

AudiopluginAudioProcessorEditor::~AudiopluginAudioProcessorEditor()
{
}

//==============================================================================
void AudiopluginAudioProcessorEditor::paint (juce::Graphics& g)
{
    //g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    g.fillAll(juce::Colour(0xffc8a1e3));
}

void AudiopluginAudioProcessorEditor::resized()
{
    tabbedComponent->setBounds(0, 0, windowWidth, windowHeight);
    //flexBox.performLayout(getLocalBounds().toFloat());

}
