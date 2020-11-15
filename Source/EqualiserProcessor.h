
#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
#include "FilterBand.h"

class Equaliser_Parameter : public juce::AudioProcessorParameter {
public:
    float defaultValue{ 0 };
    float currentValue{ 0 };
    String name;

    float getValue() const override {
        return currentValue;
    }

    void setValue(float newValue) override {
        currentValue = newValue;
    }

    float getDefaultValue() const override {
        return defaultValue;
    }

    String getName(int maximumStringLength) const override {
        return name;
    }

    String getLabel() const override {
        return getName(10);
    }

    float getValueForText(const String& text) const override {
        return 1;
    }
};

class EqualiserProcessor : public ProcessorBase {
public:
    EqualiserProcessor():
        band0(*this, "Band 0", 1000, 0, samplerate),
        band1(*this, "Band 1", 1000, 0, samplerate)
    {
    };

    ~EqualiserProcessor() {};

    void prepareToPlay(double sampleRate, int samplesPerBlock) override {
        // store this for later use
        this->samplerate = sampleRate;

        int numChannels = getTotalNumInputChannels();

        band0.prepare(numChannels);
        band1.prepare(numChannels);
    };
    void processBlock(AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
        juce::ScopedNoDenormals noDenormals; // a boilerplate snippet from JUCE, potentially increases performance on some platforms

        const int numChannels = buffer.getNumChannels();
        const int numSamples = buffer.getNumSamples();

        for (int ch = 0; ch < numChannels; ++ch)
        {
            auto* channelData = buffer.getWritePointer(ch);

            for (int i = 0; i < numSamples; ++i)
            {
                // for easier reading, copy the sample from io buffer
                // this should be optimised by the compiler
                auto sample = channelData[i];

                // Replace sample with all bands, all bands run in series
                // There's an array of Biquad objects inside each band, with the name biquad.
                // A specific Biquad object per channel is then accessed with the [ch], and performFilter is called
                // It takes a sample as an input, and returns a sample, that we replace our "sample" with.
                sample = band0.biquads[ch].performFilter(sample);
                sample = band1.biquads[ch].performFilter(sample);

                // write back to io buffer
                channelData[i] = sample;
            }
        }
    }
    FilterBand band0, band1;

private:
    double samplerate;
};