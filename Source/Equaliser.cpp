
#include "Equaliser.h"
/*
EqBandComponent::EqBandComponent(FilterBand& band)
    : freqSlider(Slider::LinearVertical,Slider::TextBoxBelow)
    , qualSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , gainSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , freqAttachment(*band.freqParam, freqSlider)
    , qualAttachment(*band.qualParam, qualSlider)
    , gainAttachment(*band.gainParam, gainSlider)
{
    freqLabel.setText("freq", dontSendNotification);
    addAndMakeVisible(freqLabel);
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(qualSlider);
    addAndMakeVisible(gainSlider);
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
*/

void Equaliser::process(float& leftSample, float& rightSample){
    juce::ScopedNoDenormals noDenormals; // a boilerplate snippet from JUCE, potentially increases performance on some platforms

    //const int numChannels = buffer.getNumChannels();
    //const int numSamples = buffer.getNumSamples();

            // for easier reading, copy the sample from io buffer
            // this should be optimised by the compiler
    //double sample;
            // Replace sample with all bands, all bands run in series
            // There's an array of Biquad objects inside each band, with the name biquad.
            // A specific Biquad object per channel is then accessed with the [ch], and performFilter is called
            // It takes a sample as an input, and returns a sample, that we replace our "sample" with.
   leftSample = biquads[0].performFilter(leftSample);
   rightSample = biquads[1].performFilter(rightSample);

            // write back to io buffer
            //channelData[i] = sample;
        
    
}

void Equaliser::prepare(int numChannels)
{
    // Allocate required number of biquads
    if (numChannels != biquads.size())
        biquads.resize(numChannels);

    parameterValueChanged(0, 0);
}

// This function is called when values change
void Equaliser::parameterValueChanged(int parameterIndex, float newValue)
{
    const float freq = freqParam->get();
    const float qual = qualParam->get();
    const float gain = gainParam->get();

    const bool bandTypeChanged = (parameterIndex == typeParam->getParameterIndex());

    if (typeParam->getIndex() == 0)
    {
        for (int ch = 0; ch < biquads.size(); ++ch)
        {
            biquads[ch].design_lowpass_filter(freq, qual, samplerate);

            if (bandTypeChanged)
                biquads[ch].clearState(); // clear state only if band type was changed
        }
    }
    else if (typeParam->getIndex() == 1)
    {
        for (int ch = 0; ch < biquads.size(); ++ch)
        {
            biquads[ch].design_peaking_filter(freq, gain, qual, samplerate);

            if (bandTypeChanged)
                biquads[ch].clearState();
        }
    }
    else
    {
        // We shouldn't be here, somethings gone terribly wrong
        jassertfalse;
    }
}