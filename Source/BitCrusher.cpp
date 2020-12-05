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
/*
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

}*/

juce::Array<float> BitCrusher::getWhiteNoise(int numSamples) {

    juce::Array<float> noiseArr;

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

            z0 = sqrtf(-2.0 * logf(u1)) * cosf(2 * float(juce::double_Pi) * u2);
            z1 = sqrtf(-2.0 * logf(u1)) * sinf(2 * float(juce::double_Pi) * u2);

            output = z0 * sigma + mu;
        }

        // NAN check ...
        jassert(output == output);
        jassert(output > -50 && output < 50);

        //
        noiseArr.add(output);

    }

    return noiseArr;

}

void BitCrusher::process(juce::AudioBuffer<float>& buffer) {
    juce::ScopedNoDenormals noDenormals;


    int numSamples = buffer.getNumSamples();

    float noiseAmt = -120 + 120 * (noise / 100); // dB
    float bitDepth = bitRedux;
    int rateDivide = rateRedux;

    // GET PARAMETERS :::::
    noiseAmt = juce::jlimit<float>(-120, 0, noiseAmt);
    noiseAmt = juce::Decibels::decibelsToGain(noiseAmt);



    // SAFETY CHECK :::: since some hosts will change buffer sizes without calling prepToPlay (Bitwig)
    if (noiseBuffer.getNumSamples() != numSamples)
    {
        noiseBuffer.setSize(2, numSamples, false, true, true); // clears
        currentOutputBuffer.setSize(2, numSamples, false, true, true); // clears
    }


    // COPY for processing ...
    currentOutputBuffer.copyFrom(0, 0, buffer.getReadPointer(0), numSamples);
    float* leftDry = buffer.getWritePointer(0);
    float* rightDry = 0;
    if (buffer.getNumChannels() > 1) {
        currentOutputBuffer.copyFrom(1, 0, buffer.getReadPointer(1), numSamples);
        rightDry = buffer.getWritePointer(1);
    }


    // BUILD NOISE ::::::
    {
        noiseBuffer.clear();

        juce::Array<float> noiseArr = getWhiteNoise(numSamples);

        // range bound
        noiseAmt = juce::jlimit<float>(0, 1, noiseAmt);

        juce::FloatVectorOperations::multiply(noiseArr.getRawDataPointer(), noiseAmt, numSamples);

        // ADD the noise ...
        juce::FloatVectorOperations::add(noiseBuffer.getWritePointer(0), noiseArr.getRawDataPointer(), numSamples);
        juce::FloatVectorOperations::add(noiseBuffer.getWritePointer(1), noiseArr.getRawDataPointer(), numSamples); // STEREO

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
        float* data = currentOutputBuffer.getWritePointer(chan);

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


    juce::FloatVectorOperations::multiply(currentOutputBuffer.getWritePointer(0), wetDry, numSamples);
    juce::FloatVectorOperations::multiply(leftDry, (1 - wetDry), numSamples);
    juce::FloatVectorOperations::add(currentOutputBuffer.getWritePointer(0), leftDry, numSamples);
    buffer.copyFrom(0, 0, currentOutputBuffer, 0, 0, numSamples);

    if (buffer.getNumChannels() > 1) {
        juce::FloatVectorOperations::multiply(currentOutputBuffer.getWritePointer(1), wetDry, numSamples);
        juce::FloatVectorOperations::multiply(rightDry, (1 - wetDry), numSamples);
        juce::FloatVectorOperations::add(currentOutputBuffer.getWritePointer(1), rightDry, numSamples);
        buffer.copyFrom(1, 0, currentOutputBuffer, 1, 0, numSamples);
    }


    //float leftSample = wetDry * leftWet + (1 - wetDry) * leftDry;
    //float rightSample = wetDry * rightWet + (1 - wetDry) * rightDry;
    // COPY to the actual output buffer :::
    //buffer.copyFrom(1, 0, currentOutputBuffer, 1, 0, numSamples);

}


/*
void BitCrusher::process(float& leftSample, float& rightSample, const int& numSamples, const int& numChannels) {
    juce::ScopedNoDenormals noDenormals;

    float noiseAmt = -120 + 120 * (noise / 100); // dB
    float bitDepth = bitRedux;
    int rateDivide = rateRedux;

    // GET PARAMETERS :::::
    noiseAmt = juce::jlimit<float>(-120, 0, noiseAmt);
    noiseAmt = juce::Decibels::decibelsToGain(noiseAmt);

    // BUILD NOISE ::::::
    float whiteNoise = getWhiteNoise();

    // range bound
    noiseAmt = juce::jlimit<float>(0, 1, noiseAmt);

    //add noise to audio
    float left = (noiseAmt * whiteNoise) + leftSample;
    float right = (noiseAmt * whiteNoise) + rightSample;

    
     //quantization
    for (int i = 0; i < numSamples; i++) {
        float totalQLevels = powf(2, bitDepth);

        float remainder = fmodf(left, 1 / totalQLevels);
        left = left - remainder;

        remainder = fmodf(right, 1 / totalQLevels);
        right = right - remainder;

        if (rateDivide > 1) {
            if (i % rateDivide != 0) {
                samplerate = samplerate / rateDivide;
            }
        }
    }

    leftSample = left;
    rightSample = right;

};
*/


void BitCrusher::parameterValueChanged(int, float)
{
    noise = noiseParam->get();
    bitRedux = bitReduxParam->get();
    rateRedux = rateReduxParam->get();
    wetDry = wetDryParam->get();
}