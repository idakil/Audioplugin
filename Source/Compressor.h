
#pragma once
#include <JuceHeader.h>
using namespace juce;

/*
Threshold: -60dB – 0 dB
Ratio: 1:1 – 30:1
Attack: 0ms – 250ms (tai niin nopea kuin mahdollista)
Release: 10 ms – 2500 ms
*/

struct Compressor : public AudioProcessorParameter::Listener {
    template <class AudioProcessorType>
    Compressor(AudioProcessorType& processor, double& fs) : samplerate(fs) {
        threshParam = new AudioParameterFloat("th", "thht", -60, 0, 5);
        slopeParam = new AudioParameterFloat("slooeParam", "slodpParam", 0, 1, 0.5);
        kneeParam = new AudioParameterFloat("kneeParam", "kneeParam", 0, 1, 0.5);
        attackParam = new AudioParameterFloat("attackParam", "attackParam", 0, 250, 0.5);
        releaseParam = new AudioParameterFloat("releaseParam", "releaseParam", 10, 2500, 0.5);

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
    void prepareToPlay(double sampleRate);
    void process(float& leftSample, float& rightSample);

    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    float compressSample(float data, float thresh, float ratio, float attack, float release, float knee);
    float interpolatePoints(float* xPoints, float* yPoints, float detectedValue);

    float tav=0.01, rms=0, gain=1;
    AudioParameterFloat* threshParam, * slopeParam, * kneeParam, * attackParam, * releaseParam;
    float thresh, slope, knee, attack, release;

    double& samplerate;

};

/*

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
*/
