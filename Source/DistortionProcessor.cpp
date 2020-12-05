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

    parameterValueChanged(NULL, NULL);
    processorChain.get<compressorIndex>().setRatio(4);
    processorChain.get<compressorIndex>().setAttack(12);
    processorChain.get<compressorIndex>().setRelease(150);

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
    processorChain.get<postSaturationGainIndex>().setGainLinear(1.0f / (saturationGain * 0.2f));
    processorChain.get<compressorIndex>().setThreshold(thresholdParam->get());
}
