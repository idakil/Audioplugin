/*
  ==============================================================================

    DistortionProcessor.h

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>


struct DistortionProcessor : public juce::AudioProcessorParameter::Listener {
    template<class AudioProcessorType>
    DistortionProcessor(AudioProcessorType& processor, double& fs) : samplerate(fs)
    {
        processor.addParameter(saturationParam = new AudioParameterFloat("saturation", "Saturation", 0.01, 100, 1));
        processor.addParameter(thresholdParam = new AudioParameterFloat("threshold", "Threshold (dB)", -60, 0, -10));
        processor.addParameter(ratioParam = new AudioParameterFloat("ratio", "Ratio", 1, 20, 4));
        processor.addParameter(attackParam = new AudioParameterFloat("attack", "Attack (ms)", 1, 30, 12));
        processor.addParameter(releaseParam = new AudioParameterFloat("release", "Release (ms)", 1, 300, 150));

        saturationParam->addListener(this);
        thresholdParam->addListener(this);
        ratioParam->addListener(this);
        attackParam->addListener(this);
        releaseParam->addListener(this);
    }

    DistortionProcessor() = delete;

    void prepareToPlay(double samplerate, int samplesPerBlock, int totalNumInputChannel);
    void process(juce::AudioBuffer<float>& buffer);

    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    enum {
        preSaturationGainIndex,
        waveshaperIndex,
        postSaturationGainIndex,
        compressorIndex
    };

    juce::AudioParameterFloat* thresholdParam;
    juce::AudioParameterFloat* ratioParam;
    juce::AudioParameterFloat* attackParam;
    juce::AudioParameterFloat* releaseParam;
    juce::AudioParameterFloat* saturationParam;

    double& samplerate;

    juce::dsp::ProcessorChain
        <
        juce::dsp::Gain<float>,
        juce::dsp::WaveShaper<float>,
        juce::dsp::Gain<float>,
        juce::dsp::Compressor<float>
        > processorChain;

};
