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
        processor.addParameter(saturationParam = new juce::AudioParameterFloat("saturation", "Saturation", 0.01f, 100.0f, 1.0f));
        processor.addParameter(thresholdParam = new juce::AudioParameterFloat("threshold", "Threshold (dB)", -60.0f, 0.0f, -10.0f));
        processor.addParameter(ratioParam = new juce::AudioParameterFloat("ratio", "Ratio", 1.0f, 20.0f, 4.0f));
        processor.addParameter(attackParam = new juce::AudioParameterFloat("attack", "Attack (ms)", 1.0f, 30.0f, 12.0f));
        processor.addParameter(releaseParam = new juce::AudioParameterFloat("release", "Release (ms)", 1.0f, 300.0f, 150.0f));

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

    juce::dsp::ProcessorChain<juce::dsp::Gain<float>, juce::dsp::WaveShaper<float>, juce::dsp::Gain<float>, juce::dsp::Compressor<float>> processorChain;
};
