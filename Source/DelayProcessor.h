/*
  ==============================================================================

    DelayProcessor.h

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayLine.h"
#include "SineOscillator.h"

struct DelayProcessor : public juce::AudioProcessorParameter::Listener {
    template <class AudioProcessorType>
    DelayProcessor(AudioProcessorType& processor, double& fs) : samplerate(fs)
    {
        //lenghtParam = new juce::AudioParameterFloat("delay_lenght", "Delay Lenght (ms)", 0.01f, 0.50f, 0.10f);
        lenghtParam = new juce::AudioParameterFloat("delay_lenght", "Delay Lenght (ms)", 1.0f, 100.0f, 1.0f);
        feedbackParam = new juce::AudioParameterFloat("delay_feedback", "Delay Feedback Amount", 0.0f, 1.0f, 0.1f);
        wetDryMixParam = new juce::AudioParameterFloat("delay_wetdry", "Chorus Wet Dry Mix", 0.0f, 100.0f, 50.0f);

        processor.addParameter(lenghtParam);
        processor.addParameter(feedbackParam);
        processor.addParameter(wetDryMixParam);

        lenghtParam->addListener(this);
        feedbackParam->addListener(this);
        wetDryMixParam->addListener(this);
    }

    DelayProcessor() = delete;

    void prepareToPlay(double sampleRate);
    void process(float& leftSample, float& rightSample);

    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    void setMouse(int x, int y);

    juce::AudioParameterFloat* lenghtParam;
    juce::AudioParameterFloat* feedbackParam;
    juce::AudioParameterFloat* wetDryMixParam;

    std::unique_ptr<DelayLine> leftDelayLine;
    std::unique_ptr<DelayLine> rightDelayLine;

    //std::unique_ptr<SineOscillator> leftLfoOsc;
    //std::unique_ptr<SineOscillator> rightLfoOsc;

    double& samplerate;

    float prevLeftDelayedSample;
    float prevRightDelayedSample;

    float delayLenght;
    float feedbackAmount;
    float wetDryMix;
};