/*
  ==============================================================================

    DelayProcessor.h

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "DelayLine.h"
#include "SineOscillator.h"

struct ChorusProcessor : public AudioProcessorParameter::Listener {
    template <class AudioProcessorType>
    ChorusProcessor(AudioProcessorType& processor, double& fs) : samplerate(fs)
    {
        lenghtParam = new AudioParameterFloat("chorus_lenght", "Delay Lenght (s)", 0.01f, 0.50f, 0.01f);
        modAmountParam = new AudioParameterFloat("chorus_modamount", "Delay Modulation (ms)", 0.0f, 20.0f, 0.10f);
        lfoSpeedParam = new AudioParameterFloat("chorus_lfospeed", "LFO Speed", 0.0f, 20.0f, 0.5f);
        wetDryMixParam = new AudioParameterFloat("chorus_wetdry", "Chorus Wet Dry Mix", 0.0f, 1.0f, 0.5f);

        processor.addParameter(lenghtParam);
        processor.addParameter(modAmountParam);
        processor.addParameter(lfoSpeedParam);
        processor.addParameter(wetDryMixParam);

        lenghtParam->addListener(this);
        modAmountParam->addListener(this);
        lfoSpeedParam->addListener(this);
        wetDryMixParam->addListener(this);
    }

    ChorusProcessor() = delete;

    void prepareToPlay(double sampleRate);
    void process(float& leftSample, float& rightSample);

    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    AudioParameterFloat* lenghtParam;
    AudioParameterFloat* modAmountParam;
    AudioParameterFloat* lfoSpeedParam;
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
    float wetDryMix;
};
