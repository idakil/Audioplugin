#include "BitCrusher.h"

/*
Bitcrusher
Mix 0,00-100,00%
Downsampling 1x-40x
Resolution 1bit-24bit
Drive 0,0-50,0dB*/

/*
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
*/
float BitCrusher::getWhiteNoise() {

    float z0 = 0;
    float z1 = 0;
    bool generate = false;

    float mu = 0; // center (0)
    float sigma = 1; // spread -1 <-> 1

    float output = 0;
    float u1 = 0;
    float u2 = 0;

    juce::Random r = juce::Random::getSystemRandom();
    r.setSeed(juce::Time::getCurrentTime().getMilliseconds());


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

            z0 = sqrtf(-2.0f * logf(u1)) * cosf(2.0f * float(juce::MathConstants<float>::twoPi) * u2);
            z1 = sqrtf(-2.0f * logf(u1)) * sinf(2.0f * float(juce::MathConstants<float>::twoPi) * u2);

            output = z0 * sigma + mu;
        }

        // NAN check ...
        jassert(output == output);
        jassert(output > -50 && output < 50);

        //
        return output;

}



void BitCrusher::process(float& leftSample, float& rightSample) {
    juce::ScopedNoDenormals noDenormals;

    float noiseAmt = -120 + 120 * (noise / 100); // dB
    float bitDepth = bitRedux;
    float rateDivide = rateRedux;

    // GET PARAMETERS :::::
    noiseAmt = juce::jlimit<float>(-120, 0, noiseAmt);
    noiseAmt = juce::Decibels::decibelsToGain(noiseAmt);

    // BUILD NOISE ::::::
    float whiteNoise = getWhiteNoise();

    // range bound
    noiseAmt = juce::jlimit<float>(0, 1, noiseAmt);

    float left = (noiseAmt * whiteNoise) + leftSample;
    float right = (noiseAmt * whiteNoise) + rightSample;

     //quantization
    float totalQLevels = powf(2, bitDepth);

    float remainder = fmodf(left, 1 / totalQLevels);
    left = left - remainder;
            
    remainder = fmodf(right, 1 / totalQLevels);
    right = right - remainder;

    if (rateDivide > 1) {
        samplerate = samplerate / rateDivide;
    }

    leftSample = left;
    rightSample = right;

    };



void BitCrusher::parameterValueChanged(int, float)
{
    noise = noiseParam->get();
    bitRedux = bitReduxParam->get();
    rateRedux = rateReduxParam->get();
}