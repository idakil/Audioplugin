
#pragma once
#include <JuceHeader.h>

struct ParameterInterface : public AudioProcessorParameter::Listener {

    template <class AudioProcessorType>
    ParameterInterface(AudioProcessorType& processor) {
        threshParam = new AudioParameterFloat("th", "thht", -60, 0, 5);
        slopeParam = new AudioParameterFloat("slooeParam", "slodpParam", 0, 1, 0.5);
        kneeParam = new AudioParameterFloat("kneeParam", "kneeParam", 0, 1, 0.5);
        attackParam = new AudioParameterFloat("attackParam", "attackParam", 0, 1, 0.5);
        releaseParam = new AudioParameterFloat("releaseParam", "releaseParam", 0, 1, 0.5);

        bitRedux = new AudioParameterFloat("bitRedux", "bitRedux", -60, 0, 5);
        rateRedux = new AudioParameterFloat("rateRedux", "rateRedux", 0, 1, 0.5);
        noise = new AudioParameterFloat("noise", "noise", 0, 1, 0.5);
        processor.addParameter(threshParam);
        processor.addParameter(slopeParam);
        processor.addParameter(kneeParam);
        processor.addParameter(attackParam);
        processor.addParameter(releaseParam);

        processor.addParameter(bitRedux);
        processor.addParameter(rateRedux);
        processor.addParameter(noise);

    }
    ~ParameterInterface() {};
    ParameterInterface() = delete;

    void prepare(int numChannels) {};

    // This function is called when values change
    // We don't actually need the parameterIndex and newValue here, so commenting the names out removes "parameter unused" compiler warning
    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) {};

    // This is part of the life of being a Parameter Listener, we need to implement unnecessary stuff due to library design in JUCE.
    // Just implement with empty body for now.
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) {};

    AudioParameterFloat* bitRedux, * rateRedux, * noise;

    AudioParameterFloat* threshParam, * slopeParam, * kneeParam, * attackParam, * releaseParam;
};