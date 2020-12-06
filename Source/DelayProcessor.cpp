/*
  ==============================================================================

    DelayProcessor.cpp

  ==============================================================================
*/

#include "DelayProcessor.h"

void DelayProcessor::prepareToPlay(double sampleRate)
{
    const int maxDelayLineInSamples = 10 * sampleRate;

    // To assign a new object to std::unique_ptr, we call its reset().
    // This deletes the old object, if one exists, and moves to point to the new object.
    leftDelayLine.reset(new DelayLine(maxDelayLineInSamples));
    rightDelayLine.reset(new DelayLine(maxDelayLineInSamples));

    //const double frequencyInHz = 0.5f;
    //const double leftStartingPhase = MathConstants<double>::twoPi * 0;
    //const double rightStartingPhase = MathConstants<double>::twoPi * 0.25;

    //leftLfoOsc.reset(new SineOscillator(sampleRate, frequencyInHz, leftStartingPhase));
    //rightLfoOsc.reset(new SineOscillator(sampleRate, frequencyInHz, rightStartingPhase));

    //leftLfoOsc->setFrequency(frequencyInHz);
    //rightLfoOsc->setFrequency(frequencyInHz);

    // These will be used for feedback, initialise to zero
    prevLeftDelayedSample = 0;
    prevRightDelayedSample = 0;

    parameterValueChanged(NULL, NULL);
}

void DelayProcessor::process(float& leftSample, float& rightSample)
{
    const float defaultModulation = 1.0;
    const float maxAmplitudeInSeconds = defaultModulation / 1000;
    const float wetDry = wetDryMix / 100.0f;
    const float lenght = delayLenght / 100.0f;

    float leftDelayInSamples = lenght * samplerate;
    float rightDelayInSamples = lenght * samplerate;

    leftDelayLine->pushSample(leftSample + prevLeftDelayedSample * feedbackAmount);
    rightDelayLine->pushSample(rightSample + prevRightDelayedSample * feedbackAmount);

    float leftWetSample = leftDelayLine->getDelayedSampleInterp(leftDelayInSamples);
    float rightWetSample = rightDelayLine->getDelayedSampleInterp(rightDelayInSamples);

    float leftDrySample = leftSample;
    float rightDrySample = rightSample;

    leftSample = wetDry * leftWetSample + (1 - wetDry) * leftDrySample;
    rightSample = wetDry * rightWetSample + (1 - wetDry) * rightDrySample;

    prevLeftDelayedSample = leftWetSample;
    prevRightDelayedSample = rightWetSample;
}

void DelayProcessor::parameterValueChanged(int, float)
{
    delayLenght = lenghtParam->get();
    wetDryMix = wetDryMixParam->get();
    feedbackAmount = feedbackParam->get();
}

void DelayProcessor::setMouse(int x, int y)
{
    delayLenght = x * 0.5f;
}
