/*
  ==============================================================================

    Compressor.h
    Created: 9 Nov 2020 11:18:56am
    Author:  idakil

  ==============================================================================
*/

#pragma once
#include "CircularBuffer.h"

class Compressor {
public:
    Compressor();
    float compressSample(float data, float thresh, float ratio, float attack, float release, float knee);
    float interpolatePoints(float* xPoints, float* yPoints, float detectedValue);
private:
    CircularBuffer buffer;
    float tav, rms, gain;
};