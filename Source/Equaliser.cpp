
#include "Equaliser.h"


void Equaliser::process(float& leftSample, float& rightSample){
    juce::ScopedNoDenormals noDenormals; // a boilerplate snippet from JUCE, potentially increases performance on some platforms

    //const int numChannels = buffer.getNumChannels();
    //const int numSamples = buffer.getNumSamples();

            // for easier reading, copy the sample from io buffer
            // this should be optimised by the compiler
    //double sample;
            // Replace sample with all bands, all bands run in series
            // There's an array of Biquad objects inside each band, with the name biquad.
            // A specific Biquad object per channel is then accessed with the [ch], and performFilter is called
            // It takes a sample as an input, and returns a sample, that we replace our "sample" with.
   leftSample = biquads[0].performFilter(leftSample);
   rightSample = biquads[1].performFilter(rightSample);

            // write back to io buffer
            //channelData[i] = sample;
        
    
}

void Equaliser::prepare(int numChannels)
{
    // Allocate required number of biquads
    if (numChannels != biquads.size())
        biquads.resize(numChannels);

    parameterValueChanged(0, 0);
}

// This function is called when values change
void Equaliser::parameterValueChanged(int parameterIndex, float newValue)
{
    const float freq = freqParam->get();
    const float qual = qualParam->get();
    const float gain = gainParam->get();

    const bool bandTypeChanged = (parameterIndex == typeParam->getParameterIndex());

    if (typeParam->getIndex() == 0)
    {
        for (int ch = 0; ch < biquads.size(); ++ch)
        {
            biquads[ch].design_lowpass_filter(freq, qual, samplerate);

            if (bandTypeChanged)
                biquads[ch].clearState(); // clear state only if band type was changed
        }
    }
    else if (typeParam->getIndex() == 1)
    {
        for (int ch = 0; ch < biquads.size(); ++ch)
        {
            biquads[ch].design_peaking_filter(freq, gain, qual, samplerate);

            if (bandTypeChanged)
                biquads[ch].clearState();
        }
    }
    else
    {
        // We shouldn't be here, somethings gone terribly wrong
        jassertfalse;
    }
}