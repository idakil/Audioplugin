
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

enum {
    thresh,
    slope,
    knee,
    attack,
    release 
};

struct CompressorComponent : public Component {

    CompressorComponent(ParameterInterface& pi);
    ~CompressorComponent() {};

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider threshSlider, slopeSlider, kneeSlider, attackSlider, releaseSlider;
    Label threshLabel, slopeLabel, kneeLabel, attackLabel, releaseLabel;
    SliderParameterAttachment threshAttachment, slopeAttachment, kneeAttachment, attackAttachment, releaseAttachment;

};

class Compressor : public Component, public ProcessorBase {
public:
    Compressor(AudiopluginAudioProcessor&);
    ~Compressor() {};
    float compressSample(float data, float thresh, float ratio, float attack, float release, float knee);
    float interpolatePoints(float* xPoints, float* yPoints, float detectedValue);
    
    void prepareToPlay(double samplerate, int samplesPerBlock);
    void processBlock(AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);


    void resized() {
        auto bounds = getLocalBounds();
        int h = bounds.getHeight() / 2;
        compressorComponent.setBounds(bounds.removeFromTop(h));
        compressorComponent.setBounds(bounds);
    }

    void effectParameters() {

    };

private:
    AudiopluginAudioProcessor& audioProcessor;
    CompressorComponent compressorComponent;

    //Array<Compressor> allCompressors;
    float tav, rms, gain;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Compressor)

};

