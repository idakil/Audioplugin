/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DelayLine.h"
#include "SineOscillator.h"

#include "ChorusProcessor.h"
#include "DelayProcessor.h"

//==============================================================================
/**
*/
class AudiopluginAudioProcessor : public juce::AudioProcessor
{
public:
    //==============================================================================
    AudiopluginAudioProcessor();
    ~AudiopluginAudioProcessor() override;

    //==============================================================================
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

#ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
#endif

    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

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
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    ChorusProcessor chorus;
    DelayProcessor delay;

private:
    //==============================================================================
    AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    AudioProcessorValueTreeState parameters;

    std::atomic<float>* delayLenghtParam = nullptr;
    std::atomic<float>* delayModAmountParam = nullptr;
    std::atomic<float>* delayLfoSpeedParam = nullptr;
    std::atomic<float>* delayFeedbackParam = nullptr;
    std::atomic<float>* delaywetDryMixParam = nullptr;

    // member variables
    float prevLeftDelayedSample;
    float prevRightDelayedSample;

    double samplerate;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudiopluginAudioProcessor);
};