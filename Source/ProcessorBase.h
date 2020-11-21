
#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

struct ProcessorBase
{
    //==============================================================================
    ProcessorBase() {}
    ~ProcessorBase() {};

    //==============================================================================
    void prepareToPlay(double, int)  {}
    void releaseResources()  {}
    void processBlock(juce::AudioSampleBuffer&, juce::MidiBuffer&) {}

    void effectParameters() {};

    //==============================================================================
};

