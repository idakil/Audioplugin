/*
  ==============================================================================

    CompressorProcessor.h
    Created: 15 Nov 2020 6:29:27pm
    Author:  idakil

  ==============================================================================
*/

#pragma once


#pragma once
#include <JuceHeader.h>
#include "ProcessorBase.h"
using namespace juce;

class Compressor_Parameter : public AudioProcessorParameter {
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

class CompressorProcessor : public ProcessorBase {
public:
    CompressorProcessor() {
        threshParam = new Compressor_Parameter();
        threshParam->defaultValue = 0;
        threshParam->currentValue = 0;
        threshParam->name = "threshold";
        addParameter(threshParam);

        slopeParam = new Compressor_Parameter();
        slopeParam->defaultValue = 0;
        slopeParam->currentValue = 0;
        slopeParam->name = "knee";
        addParameter(slopeParam);

        kneeParam = new Compressor_Parameter();
        kneeParam->defaultValue = 0;
        kneeParam->currentValue = 0;
        kneeParam->name = "kneeParam";
        addParameter(kneeParam);

        attackParam = new Compressor_Parameter();
        attackParam->defaultValue = 0;
        attackParam->currentValue = 0;
        attackParam->name = "attackParam";
        addParameter(attackParam);

        releaseParam = new Compressor_Parameter();
        releaseParam->defaultValue = 0;
        releaseParam->currentValue = 0;
        releaseParam->name = "releaseParam";
        addParameter(releaseParam);

    };
    ~CompressorProcessor() {};

    void prepareToPlay(double samplerate, int samplesPerBlock) override {
        for (int channel = 0; channel < getNumOutputChannels(); channel++) {
            allCompressors.add(Compressor());
        }

    };
    void processBlock(AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages) override {
        float at = 1 - std::pow(MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (attackParam->getValue() / 1000.0f));
        float rt = 1 - std::pow(MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (releaseParam->getValue() / 1000.0f));

        for (int i = 0; i < buffer.getNumSamples(); i++) {
            for (int channel = 0; channel < getTotalNumOutputChannels(); channel++) {
                auto* data = buffer.getWritePointer(channel);
                Compressor* comp = &allCompressors.getReference(channel);
                data[i] = comp->compressSample(data[i], threshParam->getValue(), slopeParam->getValue(), at, rt, kneeParam->getValue());
            }
        }
    }
private:
    Array<Compressor> allCompressors;
    //AudioProcessorValueTreeState state;
    Compressor_Parameter* threshParam, * slopeParam, * kneeParam, * attackParam, * releaseParam;
};