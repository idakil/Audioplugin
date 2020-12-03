#pragma once
#include <JuceHeader.h>
#include "biquad.hpp"

struct Equaliser : public juce::AudioProcessorParameter::Listener {

    template <class AudioProcessorType>
    Equaliser(AudioProcessorType& processor, juce::String bandName, int defaultType, double& fs) : samplerate(fs) {
        if (defaultType == 0) { //it's lowpass
            freqParam = new juce::AudioParameterFloat(bandName + "freqlow", bandName + " Freq", juce::NormalisableRange<float>(20, 10000, 50), 200);
            qualParam = new juce::AudioParameterFloat(bandName + "quallow", bandName + " Q", 0.1, 2.0f, 0.707);
            gainParam = new juce::AudioParameterFloat(bandName + "gainlow", bandName + " Gain", -30, 30, 0);
        }
        else {
            freqParam = new juce::AudioParameterFloat(bandName + "freqpeak", bandName + " Freq", 20, 20000, 200);
            qualParam = new juce::AudioParameterFloat(bandName + "qualpeak", bandName + " Q", 0.1, 2.0f, 0.707);
            gainParam = new juce::AudioParameterFloat(bandName + "gainpeak", bandName + " Gain", -30, 30, 0);
        }

        typeParam = new juce::AudioParameterChoice(bandName + "type", bandName + " Type", { "Lowpass", "Peaking", }, defaultType);

        // Add the newly created parameters to the audio processor
        processor.addParameter(freqParam);
        processor.addParameter(qualParam);
        processor.addParameter(gainParam);
        processor.addParameter(typeParam);

        // register as listener
        freqParam->addListener(this);
        qualParam->addListener(this);
        gainParam->addListener(this);
        typeParam->addListener(this);
    }

    Equaliser() = delete;

    void prepare(int numChannels);
    void process(float& leftSample, float& rightSample);

    // This function is called when values change
    // We don't actually need the parameterIndex and newValue here, so commenting the names out removes "parameter unused" compiler warning
    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;

    // This is part of the life of being a Parameter Listener, we need to implement unnecessary stuff due to library design in JUCE.
    // Just implement with empty body for now.
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {};

    // The actual EQ state
    // Store the biquads into the processor state. Since the filter has a state that should be
    // carried over form block to block, we can't just create a new filter in every block.
    //
    // To make this more general, we're storing the biquads into a vector and allocate the memory for them.
    std::vector<Biquad<double>> biquads;

    // Parameters that the band needs
    juce::AudioParameterFloat* freqParam, * qualParam, * gainParam;
    juce::AudioParameterChoice* typeParam;

    double& samplerate; // let's store a reference of samplerate that the AudioProcessor maintains

};