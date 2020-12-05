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
        processor.addParameter(saturationParam = new juce::AudioParameterFloat("saturation", "Saturation", 0.1f, 100.0f, 0.1f));
        processor.addParameter(thresholdParam = new juce::AudioParameterFloat("gain", "Gain (dB)", -60, 0, -10));

        saturationParam->addListener(this);
        thresholdParam->addListener(this);
    }

    DistortionProcessor() = delete;

    void prepareToPlay(double samplerate, int samplesPerBlock, int totalNumInputChannel);
    void process(juce::AudioBuffer<float>& buffer);
    void reset()
    {
        processorChain.reset();
    }

    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    enum {
        preSaturationGainIndex,
        waveshaperIndex,
        postSaturationGainIndex,
        compressorIndex
    };

    juce::AudioParameterFloat* thresholdParam;
    juce::AudioParameterFloat* saturationParam;

    double& samplerate;

    juce::dsp::ProcessorChain<juce::dsp::Gain<float>, juce::dsp::WaveShaper<float>, juce::dsp::Gain<float>, juce::dsp::Compressor<float>> processorChain;
};
