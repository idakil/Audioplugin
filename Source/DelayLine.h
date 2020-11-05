/*
  ==============================================================================
  
  From https://github.com/ollierik/juce-beginner-examples

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
using namespace juce;

class DelayLine
{
public:
    DelayLine(int maxNumSamples);
    ~DelayLine();

    // Push a new sample into the delay line
    void pushSample(double sample);

    // Get a sample that has been delayed delayInSamples amout
    double getDelayedSample(int delayInSamples);

    // Get a sample, but interpolated. We need to use this if the delay read tap moves
    double getDelayedSampleInterp(float delayInSamples);

private:

    AudioBuffer<double>* buffer;

    int writehead;
    const int maxNumSamples;
};
