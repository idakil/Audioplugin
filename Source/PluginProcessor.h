#pragma once

#include <JuceHeader.h>
#include "Equaliser.h"
#include "Compressor.h"
#include "BitCrusher.h"
#include "ChorusProcessor.h"
#include "DistortionProcessor.h"
#include "DelayProcessor.h"

class AudiopluginAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudiopluginAudioProcessor();
    ~AudiopluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

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

    Compressor compressor0;
    Compressor compressor1;

    BitCrusher bitCrusher;
    Equaliser eq0;
    Equaliser eq1;

    ChorusProcessor chorus;
    DelayProcessor delay;
    DistortionProcessor distortion;

private:
    //==============================================================================
    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    juce::AudioProcessorValueTreeState parameters;

    double samplerate;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudiopluginAudioProcessor)
};
