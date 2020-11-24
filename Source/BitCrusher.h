
#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
using namespace juce;

/*
Bitcrusher 
Mix 0,00-100,00%
Downsampling 1x-40x
Resolution 1bit-24bit
Drive 0,0-50,0dB
*/

struct BitCrusher : public AudioProcessorParameter::Listener {

    template <class AudioProcessorType>
    BitCrusher(AudioProcessorType& processor, double& fs) : samplerate(fs) {

        bitReduxParam = new AudioParameterFloat("bitRedux", "bitRedux", -60, 0, 5);
        rateReduxParam = new AudioParameterFloat("rateRedux", "rateRedux", 0, 1, 0.5);
        noiseParam = new AudioParameterFloat("noise", "noise", 0, 1, 0.5);
 
        processor.addParameter(bitReduxParam);
        processor.addParameter(rateReduxParam);
        processor.addParameter(noiseParam);

        bitReduxParam->addListener(this);
        rateReduxParam->addListener(this);
        noiseParam->addListener(this);
    }

    BitCrusher() = delete;
    void process(float& leftSample, float& rightSample, juce::AudioBuffer<float>& buffer);

    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {}

    AudioParameterFloat* bitReduxParam, * rateReduxParam, * noiseParam;
    AudioSampleBuffer noiseBuffer, currentOutputBuffer;
    float bitRedux, rateRedux, noise;
    double& samplerate;
};

//==============================================================================
/**

class BitCrusher : public Component, public ProcessorBase
{
public:
    BitCrusher(AudiopluginAudioProcessor);
    ~BitCrusher();

    void resized() {
        auto bounds = getLocalBounds();
        int h = bounds.getHeight() / 2;
        bitCrusherComponent.setBounds(bounds.removeFromTop(h));
        bitCrusherComponent.setBounds(bounds);
    }
    //==============================================================================
    void processBlock(AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void effectParameters() {

    };

private:

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudiopluginAudioProcessor& audioProcessor;
    BitCrusherComponent bitCrusherComponent;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BitCrusher)
};
*/