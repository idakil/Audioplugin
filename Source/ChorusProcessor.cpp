/*
  ==============================================================================

    DelayProcessor.cpp

  ==============================================================================
*/

#include "ChorusProcessor.h"

void ChorusProcessor::process(float& leftSample, float& rightSample)
{

    const float delayLenght = lenghtParam->get();
    const float modulation = modAmountParam->get();
    const float wetDryRatio = wetDryMixParam->get();
    const float feedbackGain = feedbackParam->get();
    const float lfoFreqInSecs = lfoSpeedParam->get();

    const float maxAmplitudeInSeconds = modulation / 1000;

    leftLfoOsc->setFrequency(lfoFreqInSecs);
    rightLfoOsc->setFrequency(lfoFreqInSecs);

    float leftDelayInSamples = delayLenght * samplerate + leftLfoOsc->getNextSample() * maxAmplitudeInSeconds * samplerate;
    float rightDelayInSamples = delayLenght * samplerate + rightLfoOsc->getNextSample() * maxAmplitudeInSeconds * samplerate;

    float leftWetSample = leftDelayLine->getDelayedSampleInterp(leftDelayInSamples);
    float rightWetSample = rightDelayLine->getDelayedSampleInterp(rightDelayInSamples);

    leftDelayLine->pushSample(leftSample + rightWetSample * feedbackGain);
    rightDelayLine->pushSample(rightSample + leftWetSample * feedbackGain);

    float leftDrySample = leftSample;
    float rightDrySample = rightSample;

    leftSample = wetDryRatio * leftWetSample + (1 - wetDryRatio) * leftDrySample;
    rightSample = wetDryRatio * rightWetSample + (1 - wetDryRatio) * rightDrySample;

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
    const double leftStartingPhase = MathConstants<double>::twoPi * 0;
    const double rightStartingPhase = MathConstants<double>::twoPi * 0.25;

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
    feedbackAmount = feedbackParam->get();
    lfoSpeed = lfoSpeedParam->get();
}

