/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Compressor.h"
#include "BitCrusherProcessor.h"
#include "EqualiserProcessor.h"
#include "CompressorProcessor.h"

//==============================================================================
/**
*/
class AudiopluginAudioProcessor  : public juce::AudioProcessor
{
public:
    using AudioGraphIOProcessor = juce::AudioProcessorGraph::AudioGraphIOProcessor;
    using Node = juce::AudioProcessorGraph::Node;
    //==============================================================================
    AudiopluginAudioProcessor();
    ~AudiopluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override
    {
        if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::disabled()
            || layouts.getMainOutputChannelSet() == juce::AudioChannelSet::disabled())
            return false;

        if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
            && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
            return false;

        return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
    }

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //FilterBand band0;
    //FilterBand band1;
    Array<AudioProcessor*> processors;

    void updateGraph() {
        mainProcessor->addNode(std::make_unique<BitCrusherProcessor>());
        mainProcessor->addNode(std::make_unique<EqualiserProcessor>());
        mainProcessor->addNode(std::make_unique<CompressorProcessor>());
        for (size_t i = 0; i < mainProcessor->getNumNodes(); i++)
        {
            processors.add(mainProcessor->getNode(i)->getProcessor());
        }
    };
private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState parameters;
    std::unique_ptr<juce::AudioProcessorGraph> mainProcessor;
    juce::AudioParameterBool* muteInput;

    //juce::StringArray processorChoices{ "Empty", "EQ", "Comp", "BitCrusher" };

    /*juce::AudioParameterChoice* eqProcessor;
    juce::AudioParameterChoice* compProcessor;
    juce::AudioParameterChoice* bcProcessor;

    juce::AudioParameterBool* eqBypass;
    juce::AudioParameterBool* compBypass;
    juce::AudioParameterBool* bcBypass;
    */

    Array<Compressor> allCompressors;
    std::atomic<float>* threshParam, * slopeParam, * kneeParam, * attackParam, * releaseParam;

    double samplerate;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudiopluginAudioProcessor)
};
