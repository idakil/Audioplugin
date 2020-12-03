/*
  ==============================================================================

    DelayProcessor.cpp

  ==============================================================================
*/

#include "ChorusProcessor.h"

void ChorusProcessor::process(float& leftSample, float& rightSample)
{
    const float maxAmplitudeInSeconds = modulationAmount / 1000;
    const float defaultFeedback = 0.1f;

    leftLfoOsc->setFrequency(lfoSpeed);
    rightLfoOsc->setFrequency(lfoSpeed);

    float leftDelayInSamples = delayLenght * samplerate + leftLfoOsc->getNextSample() * maxAmplitudeInSeconds * samplerate;
    float rightDelayInSamples = delayLenght * samplerate + rightLfoOsc->getNextSample() * maxAmplitudeInSeconds * samplerate;

    float leftWetSample = leftDelayLine->getDelayedSampleInterp(leftDelayInSamples);
    float rightWetSample = rightDelayLine->getDelayedSampleInterp(rightDelayInSamples);

    leftDelayLine->pushSample(leftSample + rightWetSample * defaultFeedback);
    rightDelayLine->pushSample(rightSample + leftWetSample * defaultFeedback);

    float leftDrySample = leftSample;
    float rightDrySample = rightSample;

    leftSample = wetDryMix * leftWetSample + (1 - wetDryMix) * leftDrySample;
    rightSample = wetDryMix * rightWetSample + (1 - wetDryMix) * rightDrySample;

    prevLeftDelayedSample = leftWetSample;
    prevRightDelayedSample = rightWetSample;
}

void ChorusProcessor::prepareToPlay(double sampleRate)
{
    const int maxDelayLineInSamples = 10 * sampleRate;

    float freqInHz = lfoSpeedParam->get();

    // To assign a new object to std::unique_ptr, we call its reset().
    // This deletes the old object, if one exists, and moves to point to the new object.
    leftDelayLine.reset(new DelayLine(maxDelayLineInSamples));
    rightDelayLine.reset(new DelayLine(maxDelayLineInSamples));

    // Create our LFOs
    const double leftStartingPhase = juce::MathConstants<double>::twoPi * 0;
    const double rightStartingPhase = juce::MathConstants<double>::twoPi * 0.25;

    leftLfoOsc.reset(new SineOscillator(sampleRate, freqInHz, leftStartingPhase));
    rightLfoOsc.reset(new SineOscillator(sampleRate, freqInHz, rightStartingPhase));

    // These will be used for feedback, initialise to zero
    prevLeftDelayedSample = 0;
    prevRightDelayedSample = 0;
}

void ChorusProcessor::parameterValueChanged(int, float)
{
    delayLenght = lenghtParam->get();
    modulationAmount = modAmountParam->get();
    wetDryMix = wetDryMixParam->get();
    lfoSpeed = lfoSpeedParam->get();
}

