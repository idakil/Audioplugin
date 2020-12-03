
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

/*
Bitcrusher 
Mix 0,00-100,00%   noise ?? ? ? ? ? ?
Downsampling 1x-40x = rateRedux ? 
Resolution 1bit-24bit = bit depth = biredux ? 
Drive 0,0-50,0dB
*/

struct BitCrusher : public juce::AudioProcessorParameter::Listener {

    template <class AudioProcessorType>
    BitCrusher(AudioProcessorType& processor, double& fs) : samplerate(fs) {

        bitReduxParam = new juce::AudioParameterFloat("bitRedux", "bitRedux", 1.0f, 40.0f, 32.0f);
        rateReduxParam = new juce::AudioParameterFloat("rateRedux", "rateRedux", 1.0f, 40.0f, 1.0f);
        noiseParam = new juce::AudioParameterFloat("noise", "noise", 0.0f, 100.0f, 0.0f);
 
        processor.addParameter(bitReduxParam);
        processor.addParameter(rateReduxParam);
        processor.addParameter(noiseParam);

        bitReduxParam->addListener(this);
        rateReduxParam->addListener(this);
        noiseParam->addListener(this);

        epsilon = std::numeric_limits<float>::min();

    }

    BitCrusher() = delete;
    void process(float& leftSample, float& rightSample);
    void prepare() {
        bitRedux = 32;
        rateRedux = 1;
        noise = 0;
    }
    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}
    float getWhiteNoise();
    float epsilon;

    juce::AudioParameterFloat* bitReduxParam, * rateReduxParam, * noiseParam;
    juce::AudioSampleBuffer noiseBuffer, currentOutputBuffer;
    float bitRedux, rateRedux, noise;
    double& samplerate;
};