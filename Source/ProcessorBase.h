
#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

class ProcessorBase
{
public:
    //==============================================================================
    ProcessorBase() {}

    //==============================================================================
    void prepareToPlay(double, int)  {}
    void releaseResources()  {}
    void processBlock(juce::AudioSampleBuffer&, juce::MidiBuffer&) {}

    void effectParameters() {
    };
private:
    //==============================================================================

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ProcessorBase)
};
