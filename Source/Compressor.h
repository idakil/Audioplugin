
#pragma once
#include <JuceHeader.h>
#include <cmath>

using namespace juce;

/*
Threshold: -60dB – 0 dB
Ratio: 1:1 – 30:1
Attack: 0ms – 250ms (tai niin nopea kuin mahdollista)
Release: 10 ms – 2500 ms
*/
/*
class PeakLevelDetector {
public:
    PeakLevelDetector::PeakLevelDetector(float sampleRate) {
        setDetector(sampleRate);
    }
    PeakLevelDetector::~PeakLevelDetector() {}
    float PeakLevelDetector::tick(float inputSample) {
        inputAbs = std::abs(inputSample);

        if (inputAbs > peakOutput) {
            b0 = b0Attack;
        }
        else {
            b0 = b0Release;
        }
        // Simplified filter equation (out = b0 * input + a1 * lastOut)
        peakOutput += b0 * (inputAbs - peakOutput);
        return peakOutput;
    }
    void PeakLevelDetector::setDetector(float sampleRate) {
        fs = sampleRate;
        peakOutput = 0.f;
        // set coefficients for leaky integrator
        b0Attack = 1.f;
        a1 = expf(-1 / (releaseTime * fs));
        b0Release = 1.f - a1;
    }
private:
    float fs, inputAbs, peakOutput;
    float b0Attack, b0Release, b0, a1;
    float releaseTime = 0.100f;     // seconds
};

class GainDynamics {
public:
    // Times are in seconds (e.g. 100ms = 0.1f, 1.2s = 1.2f)
    GainDynamics::GainDynamics(float sampleRate, float newAttackTime, float newReleaseTime) {
        attackTime = newAttackTime;
        releaseTime = newReleaseTime;
        setDetector(sampleRate);
    }
    GainDynamics::~GainDynamics() {}
    float GainDynamics::tick(float inputGain) {
        if (inputGain < outputGain) {   // Isn't this suppose to be (input > lastOutput)?
            b0 = b0Attack;
        }
        else {
            b0 = b0Release;
        }
        // Simplified filter equation (out = b0 * input + a1 * lastOut)
        outputGain += b0 * (inputGain - outputGain);
        return outputGain;
    }
    void GainDynamics::setDetector(float sampleRate) {
        fs = sampleRate;
        outputGain = 0.f;
        setAttack(attackTime);
        setRelease(releaseTime);
    }

    void GainDynamics::setAttack(float newAttackTime) {
        attackTime = newAttackTime;
        b0Attack = 1. - expf(-1. / (attackTime * fs));;
    }

    void GainDynamics::setRelease(float newReleaseTime) {
        releaseTime = newReleaseTime;
        b0Release = 1. - expf(-1. / (releaseTime * fs));;
    }
private:
    float fs, outputGain;
    double b0Attack, b0Release, b0;
    float attackTime, releaseTime;     // in seconds
};
*/

struct Compressor : public AudioProcessorParameter::Listener {
    template <class AudioProcessorType>
    Compressor(AudioProcessorType& processor, double& fs) : samplerate(fs) {
        threshParam = new AudioParameterFloat("th", "thht", -60.0f, 0.0f, -0.0f);
        slopeParam = new AudioParameterFloat("slooeParam", "slodpParam", 1.0f, 30.0f, 10.0f);
        kneeParam = new AudioParameterFloat("kneeParam", "kneeParam", 0.0f, 1.0f, 0.5f);
        attackParam = new AudioParameterFloat("attackParam", "attackParam", 0.0f, 250.0f, 100.0f);
        releaseParam = new AudioParameterFloat("releaseParam", "releaseParam", 10.0f, 2500.0f, 15.0f);

        processor.addParameter(threshParam);
        processor.addParameter(slopeParam);
        processor.addParameter(kneeParam);
        processor.addParameter(attackParam);
        processor.addParameter(releaseParam);

        threshParam->addListener(this);
        slopeParam->addListener(this);
        kneeParam->addListener(this);
        attackParam->addListener(this);
        releaseParam->addListener(this);


    }

    Compressor() = delete;
    void prepare(int samplesPerBlock);
    void process(float& leftSample, float& rightSample);

    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    float compressSample(float& data);
    float interpolatePoints(float* xPoints, float* yPoints, float detectedValue);

    float dB(float x) { 
        return 20.0 * ((x) > 0.00001 ? log10(x) : -5.0);
    }
    float dB2mag(float x) { 
        return pow(10.0, (x) / 20.0);
    }


    float tav, rms, gain;
    AudioParameterFloat* threshParam, * slopeParam, * kneeParam, * attackParam, * releaseParam;
    float thresh, slope, knee, attack, release;
    double& samplerate;
    std::vector<Compressor> allCompressors;
    //float peakOutL, peakOutR, peakSum, peakSumDb;
    //float gain, gainDb;

    //ScopedPointer<PeakLevelDetector> leftLevelDetector, rightLevelDetector;
    //ScopedPointer<GainDynamics> gainDymanics;
};

