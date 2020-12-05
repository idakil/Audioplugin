#include "Compressor.h"

/*
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

float Compressor::compressSample(float& data)
{
    float at = 1 - std::pow(juce::MathConstants<float>::euler, ((1 / samplerate) * -2.2f) / (attack / 1000.0f));
    float rt = 1 - std::pow(juce::MathConstants<float>::euler, ((1 / samplerate) * -2.2f) / (release / 1000.0f));

    rms = (1 - tav) * rms + tav * std::pow(data, 2.0f);
    float dbRMS = 10 * std::log10(rms);

    float slope = 1 - (1 / ratio);

    float dbGain = std::min(0.0f, (slope * (thresh - dbRMS)));
    float newGain = std::pow(10, dbGain / 20);

    float coeff;
    if (newGain < gain) coeff = at;
    else coeff = rt;
    gain = (1 - coeff) * gain + coeff * newGain;

    float compressedSample = gain * data;
    compressedSample = compressedSample * wetDry;
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

    float leftWet = compressSample(leftSample);
    float rightWet = compressSample(rightSample);
    float leftDry = leftSample;
    float rightDry = rightSample;
    leftSample = wetDry * leftWet + (1 - wetDry) * leftDry;
    rightSample = wetDry * rightWet + (1 - wetDry) * rightDry;
}
  
void Compressor::prepare(int numChannels) {
    parameterValueChanged(NULL, NULL);
}

void Compressor::parameterValueChanged(int, float)
{
    thresh = threshParam->get();
    ratio = ratioParam->get();
    attack = attackParam->get();
    release = releaseParam->get();
    gain = gainParam->get();
    wetDry = wetDryParam->get();
}

/*
void Compressor::process(float& leftSample, float& rightSample) {

        // Peak detector
        peakOutL = leftLevelDetector->tick(leftSample);
        peakOutR = rightLevelDetector->tick(rightSample);
        peakSum = (peakOutL + peakOutR) * 0.5f;

        // Convert to db
        peakSumDb = dB(peakSum);

        // Calculate gain
        if (peakSumDb < thresh) {
            gainDb = 0.f;
        }
        else {
            gainDb = -(peakSumDb - thresh) * (1.f - 1.f / slope);
        }

        // Gain dynamics (attack and release)
        gainDb = gainDymanics->tick(gainDb);

        // Convert to Linear
        gain = dB2mag(gainDb);

        // Apply gain
        leftSample *= gain;
        rightSample *= gain;
}

void Compressor::prepare(int samplesPerBlock) {
    gain = 1.f;

    if (leftLevelDetector == nullptr && rightLevelDetector == nullptr) {
        leftLevelDetector = new PeakLevelDetector(samplerate);
        rightLevelDetector = new PeakLevelDetector(samplerate);
    }
    else {
        leftLevelDetector->setDetector(samplerate);
        rightLevelDetector->setDetector(samplerate);
    }

    if (gainDymanics == nullptr) {
        gainDymanics = new GainDynamics(samplerate, attack, release);
    }
    else {
        gainDymanics->setDetector(samplerate);
    }
}
*/

