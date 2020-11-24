
#pragma once
#include "BitCrusher.h"

/*
BitCrusherComponent::BitCrusherComponent(ParameterInterface& pi)
    : bitReduxSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , rateReduxSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , noiseSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , bitReduxAttachment(*pi.bitRedux, bitReduxSlider)
    , rateReduxAttachment(*pi.rateRedux, rateReduxSlider)
    , noiseAttachment(*pi.noise, noiseSlider)

{
    addAndMakeVisible(rateReduxSlider);
    addAndMakeVisible(bitReduxSlider);
    addAndMakeVisible(noiseSlider);
}

BitCrusher::BitCrusher(AudiopluginAudioProcessor& p)
    : audioProcessor(p), bitCrusherComponent(p.pi)
{


    effectParameters();
    addAndMakeVisible(bitCrusherComponent);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize(400, 300);
}

BitCrusher::~BitCrusher()
{
}
*/
/*
void BitCrusher::sliderValueChanged(Slider* slider)
{

    if (slider == &noise)
    {
        processor.getParameters()[0]->setValue(slider->getValue());
    }
    else if (slider == &rateRedux)
    {
        processor.getParameters()[1]->setValue(slider->getValue());

    }
    else if (slider == &bitRedux)
    {
        processor.getParameters()[2]->setValue(slider->getValue());

    }


}*/


//==============================================================================
/*
void BitCrusherComponent::paint(Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll(Colours::midnightblue.withMultipliedBrightness(.4));

    g.setColour(Colours::beige);

    int labelW = 100;
    g.drawText("Noise", noiseSlider.getX() + noiseSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);

    g.drawText("Bit", bitReduxSlider.getX() + bitReduxSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);

    g.drawText("Rate", rateReduxSlider.getX() + rateReduxSlider.getWidth() / 2 - labelW / 2, 10, labelW, 20, Justification::centred);


}

void BitCrusherComponent::resized()
{

    int margin = 10;
    int w = 60;
    int y = 50;

    noiseSlider.setBounds(getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);

    bitReduxSlider.setBounds(2 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);

    rateReduxSlider.setBounds(3 * getWidth() / 4 - w / 2, y, w, getHeight() - y - margin);
}
*/

static Array<float> getSimpleNoise(int numSamples)
{
    Random r = Random::getSystemRandom();
    Array<float> noise;

    for (int s = 0; s < numSamples; s++)
    {
        noise.add((r.nextFloat() - .5) * 2);
    }

    return noise;

}

static Array<float> getWhiteNoise(int numSamples) {

    Array<float> noise;

    float z0 = 0;
    float z1 = 0;
    bool generate = false;

    float mu = 0; // center (0)
    float sigma = 1; // spread -1 <-> 1

    float output = 0;
    float u1 = 0;
    float u2 = 0;

    Random r = Random::getSystemRandom();
    r.setSeed(Time::getCurrentTime().getMilliseconds());
    const float epsilon = std::numeric_limits<float>::min();

    for (int s = 0; s < numSamples; s++)
    {

        // GENERATE ::::
        // using box muller method
        // https://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
        generate = !generate;

        if (!generate)
            output = z1 * sigma + mu;
        else
        {
            do
            {
                u1 = r.nextFloat();
                u2 = r.nextFloat();
            } while (u1 <= epsilon);

            z0 = sqrtf(-2.0 * logf(u1)) * cosf(2 * float(double_Pi) * u2);
            z1 = sqrtf(-2.0 * logf(u1)) * sinf(2 * float(double_Pi) * u2);

            output = z0 * sigma + mu;
        }

        // NAN check ...
        jassert(output == output);
        jassert(output > -50 && output < 50);

        //
        noise.add(output);

    }

    return noise;

}



void BitCrusher::process(float& leftSample, float& rightSample, juce::AudioBuffer<float>& buffer) {
        ScopedNoDenormals noDenormals;
       int numSamples = buffer.getNumSamples();

        float noiseAmt = -120 + 120 *(noise / 100); // dB
        float bitDepth = bitRedux;
        int rateDivide = rateRedux;

        // GET PARAMETERS :::::
        noiseAmt = jlimit<float>(-120, 0, noiseAmt);
        noiseAmt = Decibels::decibelsToGain(noiseAmt);

        // COPY for processing ...
        currentOutputBuffer.copyFrom(0, 0, buffer.getReadPointer(0), numSamples);
        if (buffer.getNumChannels() > 1) currentOutputBuffer.copyFrom(1, 0, buffer.getReadPointer(1), numSamples);
        
        // BUILD NOISE ::::::
        {
            noiseBuffer.clear();

            Array<float> noise = getWhiteNoise(numSamples);

            // range bound
            noiseAmt = jlimit<float>(0, 1, noiseAmt);

            FloatVectorOperations::multiply(noise.getRawDataPointer(), noiseAmt, numSamples);

            // ADD the noise ...
            FloatVectorOperations::add(noiseBuffer.getWritePointer(0), noise.getRawDataPointer(), numSamples);
            FloatVectorOperations::add(noiseBuffer.getWritePointer(1), noise.getRawDataPointer(), numSamples); // STEREO

            // MULTIPLY MODE :::::
            // Multiply the noise by the signal ... so 0 signal -> 0 noise
            //        {
            //            FloatVectorOperations::multiply(noiseBuffer.getWritePointer(0), currentOutputBuffer.getWritePointer(0), numSamples);
            //            FloatVectorOperations::multiply(noiseBuffer.getWritePointer(1), currentOutputBuffer.getWritePointer(1), numSamples);
            //        }

        }

        // ADD NOISE to the incoming AUDIO ::::
        currentOutputBuffer.addFrom(0, 0, noiseBuffer.getReadPointer(0), numSamples);
        currentOutputBuffer.addFrom(1, 0, noiseBuffer.getReadPointer(1), numSamples);

        // RESAMPLE AS NEEDED :::::
        for (int chan = 0; chan < currentOutputBuffer.getNumChannels(); chan++)
        {
            float* data =currentOutputBuffer.getWritePointer(chan);

            for (int i = 0; i < numSamples; i++)
            {
                // REDUCE BIT DEPTH :::::
                float totalQLevels = powf(2, bitDepth);
                float val = data[i];
                float remainder = fmodf(val, 1 / totalQLevels);

                // Quantize ...
                data[i] = val - remainder;

                if (rateDivide > 1)
                {
                    if (i % rateDivide != 0) data[i] = data[i - i % rateDivide];
                }
            }
        }

        // COPY to the actual output buffer :::
        buffer.copyFrom(0, 0, currentOutputBuffer, 0, 0, numSamples);
        buffer.copyFrom(1, 0, currentOutputBuffer, 1, 0, numSamples);
    };

void BitCrusher::parameterValueChanged(int, float)
{
    noise = noiseParam->get();
    bitRedux = bitReduxParam->get();
    rateRedux = rateReduxParam->get();
}