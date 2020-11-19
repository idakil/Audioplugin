
#include "Equaliser.h"

EqBandComponent::EqBandComponent(FilterBand& band)
    : freqSlider(Slider::LinearVertical,Slider::TextBoxBelow)
    , qualSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , gainSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , freqAttachment(*band.freqParam, freqSlider)
    , qualAttachment(*band.qualParam, qualSlider)
    , gainAttachment(*band.gainParam, gainSlider)
{
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(qualSlider);
    addAndMakeVisible(gainSlider);
}

void EqBandComponent::paint(juce::Graphics& g)
{
}

void EqBandComponent::resized()
{
    auto bounds = getLocalBounds();
    const int third = bounds.getWidth() / 3;

    freqSlider.setBounds(bounds.removeFromLeft(third));
    qualSlider.setBounds(bounds.removeFromLeft(third));
    gainSlider.setBounds(bounds);
}

Equaliser::Equaliser(AudiopluginAudioProcessor& p)
    : audioProcessor(p)
    , bandKnobs0(p.band0)
    , bandKnobs1(p.band1)
{
    addAndMakeVisible(bandKnobs0);
    addAndMakeVisible(bandKnobs1);
    setSize(400, 300);
}

Equaliser::~Equaliser()
{
}


void Equaliser::processBlock(AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    juce::ScopedNoDenormals noDenormals; // a boilerplate snippet from JUCE, potentially increases performance on some platforms

    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();

    for (int ch = 0; ch < numChannels; ++ch)
    {
        auto* channelData = buffer.getWritePointer(ch);

        for (int i = 0; i < numSamples; ++i)
        {
            // for easier reading, copy the sample from io buffer
            // this should be optimised by the compiler
            auto sample = channelData[i];

            // Replace sample with all bands, all bands run in series
            // There's an array of Biquad objects inside each band, with the name biquad.
            // A specific Biquad object per channel is then accessed with the [ch], and performFilter is called
            // It takes a sample as an input, and returns a sample, that we replace our "sample" with.
            sample = audioProcessor.band0.biquads[ch].performFilter(sample);
            sample = audioProcessor.band1.biquads[ch].performFilter(sample);

            // write back to io buffer
            channelData[i] = sample;
        }
    }
}

void Equaliser::pluginParameters(int index, float parameter) {

};