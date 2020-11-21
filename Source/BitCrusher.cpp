
#pragma once
#include "BitCrusher.h"


BitCrusherComponent::BitCrusherComponent(ParameterInterface& pi)
    : bitReduxSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , rateReduxSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , noiseSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , bitReduxAttachment(*pi.bitRedux, bitReduxSlider)
    , rateReduxAttachment(*pi.rateRedux, rateReduxSlider)
    , noiseAttachment(*pi.noise, noiseSlider)

{
    addAndMakeVisible(rateReduxSlider);
    addAndMakeVisible(bitReduxSlider);
    addAndMakeVisible(noiseSlider);
}

BitCrusher::BitCrusher(AudiopluginAudioProcessor& p)
    : audioProcessor(p), bitCrusherComponent(p.pi)
{


    effectParameters();
    addAndMakeVisible(bitCrusherComponent);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
}

BitCrusher::~BitCrusher()
{
}

/*
void BitCrusher::sliderValueChanged(Slider* slider)
{

    if (slider == &noise)
    {
        processor.getParameters()[0]->setValue(slider->getValue());
    }
    else if (slider == &rateRedux)
    {
        processor.getParameters()[1]->setValue(slider->getValue());

    }
    else if (slider == &bitRedux)
    {
        processor.getParameters()[2]->setValue(slider->getValue());

    }


}*/


//==============================================================================
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


}