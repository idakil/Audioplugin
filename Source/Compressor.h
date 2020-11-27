
#pragma once
#include <JuceHeader.h>
#include <cmath>
#include <juce_dsp/juce_dsp.h>
using namespace juce;

/*
Threshold: -60dB – 0 dB
Ratio: 1:1 – 30:1
Attack: 0ms – 250ms (tai niin nopea kuin mahdollista)
Release: 10 ms – 2500 ms
*/


struct Compressor : public AudioProcessorParameter::Listener {
    template <class AudioProcessorType>
    Compressor(AudioProcessorType& processor, juce::String name, double& fs) : samplerate(fs) {
        threshParam = new AudioParameterFloat(name + "th", name + "thht", -60.0f, 0.0f, -12.0f);
        ratioParam = new AudioParameterFloat( name + "ratio_param", name + "ratio", 1.0f, 30.0f, 2.0f);
        attackParam = new AudioParameterFloat(name + "attackParam", name + "attackParam", 0.01f, 250.0f, 10.0f);
        releaseParam = new AudioParameterFloat(name + "releaseParam", name + "releaseParam", 10.0f, 2500.0f, 200.0f);

        processor.addParameter(threshParam);
        processor.addParameter(ratioParam);
        processor.addParameter(attackParam);
        processor.addParameter(releaseParam);

        threshParam->addListener(this);
        ratioParam->addListener(this);
        attackParam->addListener(this);
        releaseParam->addListener(this);

        tav = 0.01f;
        rms = 0.0f;
        gain = 1.0f;
    }


    Compressor() = delete;
    void prepare(int samplesPerBlock);
    void process(float& leftSample, float& rightSample);

    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    float compressSample(float& data);
    float interpolatePoints(float* xPoints, float* yPoints, float detectedValue);

    void changeParams(float& distX, float& distY) {

        Array<float> params = { thresh, ratio, attack, release };

        for (int i = 0; i < params.size(); i++)
        {
            params.set(i, params[i]* ((distX+distY)*0.5));

        };
        
        Logger::outputDebugString(std::to_string(thresh));


    }

    float prevX, prevY;
    float tav, rms, gain;
    AudioParameterFloat* threshParam, * ratioParam, * attackParam, * releaseParam;
    float thresh, ratio, attack, release;
    double& samplerate;

};

