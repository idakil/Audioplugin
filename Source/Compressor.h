
#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
using namespace juce;

/*
Threshold: -60dB – 0 dB
Ratio: 1:1 – 30:1
Attack: 0ms – 250ms (tai niin nopea kuin mahdollista)
Release: 10 ms – 2500 ms
*/


struct CompressorComponent : public Component {

    CompressorComponent(HashMap<String, AudioParameterFloat> compParams);

    void paint(juce::Graphics& g) override;
    void resized() override;
    Slider threshSlider, slopeSlider, kneeSlider, attackSlider, releaseSlider;
    Label threshLabel, slopeLabel, kneeLabel, attackLabel, releaseLabel;
    SliderParameterAttachment threshAttachment, slopeAttachment, kneeAttachment, attackAttachment, releaseAttachment;
};

class Compressor : public Component, public ProcessorBase {
public:
    Compressor(AudiopluginAudioProcessor& audioProcessor);
    float compressSample(float data, float thresh, float ratio, float attack, float release, float knee);
    float interpolatePoints(float* xPoints, float* yPoints, float detectedValue);
    
    void prepareToPlay(double samplerate, int samplesPerBlock);
    void processBlock(AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);

    void effectParameters() {

        compParams.set("threshParam", AudioParameterFloat("th", "thht", 0, 1, 0.5));
        compParams.set("slopeParam", AudioParameterFloat("slooeParam", "slodpParam", 0, 1, 0.5));
        compParams.set("kneeParam",  AudioParameterFloat("kneeParam", "kneeParam", 0, 1, 0.5));
        compParams.set("attackParam",  AudioParameterFloat("attackParam", "attackParam", 0, 1, 0.5));
        compParams.set("releaseParam", AudioParameterFloat("releaseParam", "releaseParam", 0, 1, 0.5));
    };

private:
    AudiopluginAudioProcessor& audioProcessor;
    CompressorComponent compressorComponent;
    Array<Compressor> allCompressors;
    float tav, rms, gain;
    HashMap<String, AudioParameterFloat> compParams;

    //AudioParameterFloat* threshParam, * slopeParam, *kneeParam, *attackParam, *releaseParam;
};

