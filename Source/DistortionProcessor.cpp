/*
  ==============================================================================

    DistortionProcessor.cpp

  ==============================================================================
*/

#include "DistortionProcessor.h"


void DistortionProcessor::prepareToPlay(double samplerate, int samplesPerBlock, int totalNumInputChannel)
{
    juce::dsp::ProcessSpec spec{ samplerate, (juce::uint32) samplesPerBlock, (juce::uint32) totalNumInputChannel };
    processorChain.prepare(spec);

    auto& waveshaper = processorChain.get<waveshaperIndex>();

    waveshaper.functionToUse = [](float x)
    {
        return std::tanh(x);
    };
}

void DistortionProcessor::process(juce::AudioBuffer<float>& buffer)
{
    juce::dsp::AudioBlock<float> audioBlock(buffer);
    juce::dsp::ProcessContextReplacing<float> context(audioBlock);

    processorChain.process(context);
}

void DistortionProcessor::parameterValueChanged(int, float)
{
    const float saturationGain = saturationParam->get();;

    processorChain.get<preSaturationGainIndex>().setGainLinear(saturationGain);
    processorChain.get<postSaturationGainIndex>().setGainLinear(1.0f / saturationGain);
    processorChain.get<compressorIndex>().setThreshold(thresholdParam->get());
    processorChain.get<compressorIndex>().setRatio(ratioParam->get());
    processorChain.get<compressorIndex>().setAttack(attackParam->get());
    processorChain.get<compressorIndex>().setRelease(releaseParam->get());
}
