#pragma once
#include "Compressor.h"
/*
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

    threshSlider.setBounds(getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    slopeSlider.setBounds(2 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    kneeSlider.setBounds(3 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    attackSlider.setBounds(3 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
    releaseSlider.setBounds(3 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
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
*/
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

void Compressor::process(float& leftSample, float& rightSample) {
    float at = 1 - std::pow(MathConstants<float>::euler, ((1 / samplerate) * -2.2f) / (attack / 1000.0f));
    float rt = 1 - std::pow(MathConstants<float>::euler, ((1 / samplerate) * -2.2f) / (release / 1000.0f));

    compressSample(leftSample, thresh, slope, at, rt, knee);
    compressSample(rightSample,thresh, slope, at, rt, knee);

}
void Compressor::prepareToPlay(double samplerate) {

};

void Compressor::parameterValueChanged(int, float)
{
    thresh = threshParam->get();
    slope = slopeParam->get();
    knee = kneeParam->get();
    attack = attackParam->get();
    release = releaseParam->get();
}