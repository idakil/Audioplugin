
#include "Equaliser.h"


void Equaliser::process(float& leftSample, float& rightSample){
    juce::ScopedNoDenormals noDenormals; // a boilerplate snippet from JUCE, potentially increases performance on some platforms
    
    float leftWet = biquads[0].performFilter(leftSample);
    float rightWet = biquads[1].performFilter(rightSample);
    float leftDry = leftSample;
    float rightDry = rightSample;
    leftSample = wetDry * leftWet + (1 - wetDry) * leftDry;
    rightSample = wetDry * rightWet + (1 - wetDry) * rightDry;
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

    freq = freqParam->get();
    qual = qualParam->get();
    gain = gainParam->get();
    wetDry = wetDryParam->get();
    
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