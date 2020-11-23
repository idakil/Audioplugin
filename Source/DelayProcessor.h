/*
  ==============================================================================

    DelayProcessor.h

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "DelayLine.h"
#include "SineOscillator.h"

struct DelayProcessor : public AudioProcessorParameter::Listener {
    template <class AudioProcessorType>
    DelayProcessor(AudioProcessorType& processor, double& fs) : samplerate(fs)
    {
        lenghtParam = new AudioParameterFloat("delay_lenght", "Delay Lenght (s)", 0.01f, 1.00f, 0.01f);
        modAmountParam = new AudioParameterFloat("delay_modamount", "Delay Modulation (ms)", 0.0f, 5.0f, 0.10f);
        lfoSpeedParam = new AudioParameterFloat("delay_lfospeed", "LFO Speed", 0.0f, 5.0f, 0.5f);
        feedbackParam = new AudioParameterFloat("delay_feedback", "Delay Feedback Amount", 0.0f, 1.0f, 0.1f);
        wetDryMixParam = new AudioParameterFloat("delay_wetdry", "Delay Wet Dry Mix", 0.0f, 1.0f, 0.5f);

        processor.addParameter(lenghtParam);
        processor.addParameter(modAmountParam);
        processor.addParameter(lfoSpeedParam);
        processor.addParameter(feedbackParam);
        processor.addParameter(wetDryMixParam);

        lenghtParam->addListener(this);
        modAmountParam->addListener(this);
        lfoSpeedParam->addListener(this);
        feedbackParam->addListener(this);
        wetDryMixParam->addListener(this);
    }

    DelayProcessor() = delete;

    void prepareToPlay(double sampleRate);
    void process(float& leftSample, float& rightSample);

    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    AudioParameterFloat* lenghtParam;
    AudioParameterFloat* modAmountParam;
    AudioParameterFloat* lfoSpeedParam;
    AudioParameterFloat* feedbackParam;
    AudioParameterFloat* wetDryMixParam;

    std::unique_ptr<DelayLine> leftDelayLine;
    std::unique_ptr<DelayLine> rightDelayLine;

    std::unique_ptr<SineOscillator> leftLfoOsc;
    std::unique_ptr<SineOscillator> rightLfoOsc;

    double& samplerate;

    float prevLeftDelayedSample;
    float prevRightDelayedSample;

    float delayLenght;
    float modulationAmount;
    float lfoSpeed;
    float feedbackAmount;
    float wetDryMix;
};