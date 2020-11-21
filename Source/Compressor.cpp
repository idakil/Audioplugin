#pragma once
#include "Compressor.h"

CompressorComponent::CompressorComponent(ParameterInterface& pi)
    : threshSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , slopeSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , kneeSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , attackSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , releaseSlider(Slider::LinearVertical, Slider::TextBoxBelow)

    , threshAttachment(*pi.threshParam, threshSlider)
    , slopeAttachment(*pi.slopeParam, slopeSlider)
    , kneeAttachment(*pi.kneeParam, kneeSlider)
    , attackAttachment(*pi.attackParam, attackSlider)
    , releaseAttachment(*pi.releaseParam, releaseSlider)
{
    addAndMakeVisible(threshSlider);
    addAndMakeVisible(slopeSlider);
    addAndMakeVisible(kneeSlider);
    addAndMakeVisible(attackSlider);
    addAndMakeVisible(releaseSlider);
}

void CompressorComponent::paint(juce::Graphics& g) {

};
void CompressorComponent::resized() {
    auto bounds = getLocalBounds();
    const int third = bounds.getWidth() / 3;

    threshSlider.setBounds(bounds.removeFromLeft(third));
    slopeSlider.setBounds(bounds.removeFromLeft(third));
    kneeSlider.setBounds(bounds.removeFromLeft(third));
    attackSlider.setBounds(bounds);
    releaseSlider.setBounds(bounds);
};

Compressor::Compressor(AudiopluginAudioProcessor& p)
    : audioProcessor(p)
    , compressorComponent(p.pi)
{
    tav = 0.01;
    rms = 0;
    gain = 1;
    effectParameters();
    addAndMakeVisible(compressorComponent);
    setSize(400, 300);
}

float Compressor::compressSample(float data, float thresh, float ratio, float attack, float release, float knee)
{
    rms = (1 - tav) * rms + tav * std::pow(data, 2.0f);
    float dbRMS = 10 * std::log10(rms);

    float slope = 1 - (1 / ratio);

    if (knee > 0 && dbRMS > (thresh - knee / 2.0) && dbRMS < (thresh + knee / 2.0)) {
        float kneeBottom = thresh - knee / 2.0, kneeTop = thresh + knee / 2.0;
        float xPoints[2], yPoints[2];
        xPoints[0] = kneeBottom;
        xPoints[1] = kneeTop;
        xPoints[1] = std::fmin(0.0f, kneeTop);
        yPoints[0] = 0.0f;
        yPoints[1] = slope;
        slope = interpolatePoints(&xPoints[0], &yPoints[0], thresh);
        thresh = kneeBottom;
    }

    float dbGain = std::min(0.0f, (slope * (thresh - dbRMS)));
    float newGain = std::pow(10, dbGain / 20);

    float coeff;
    if (newGain < gain) coeff = attack;
    else coeff = release;
    gain = (1 - coeff) * gain + coeff * newGain;

    float compressedSample = gain; 

    return compressedSample;
}

float Compressor::interpolatePoints(float* xPoints, float* yPoints, float detectedValue) {
    float result = 0.0f;
    int n = 2;

    for (int i = 0; i < n; i++)
    {
        float term = 1.0f;
        for (int j = 0; j < n; j++)
        {
            if (j != i) {
                term *= (detectedValue - xPoints[j]) / (xPoints[i] - xPoints[j]);
            }
        }
        result += term * yPoints[i];
    }

    return result;

}

void Compressor::processBlock(AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) {
    float at = 1 - std::pow(MathConstants<float>::euler, ((1 / audioProcessor.getSampleRate()) * -2.2f) / (audioProcessor.pi.attackParam->get() / 1000.0f));
    float rt = 1 - std::pow(MathConstants<float>::euler, ((1 / audioProcessor.getSampleRate()) * -2.2f) / (audioProcessor.pi.releaseParam->get() / 1000.0f));

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        for (int channel = 0; channel < audioProcessor.getTotalNumOutputChannels(); channel++) {
            auto* data = buffer.getWritePointer(channel);
            data[i] = compressSample(data[i], audioProcessor.pi.threshParam->get(), audioProcessor.pi.slopeParam->get(), at, rt, audioProcessor.pi.kneeParam->get());
        }
    }
}
void Compressor::prepareToPlay(double samplerate, int samplesPerBlock) {

};