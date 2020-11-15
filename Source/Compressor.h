/*
  ==============================================================================

    Compressor.h
    Created: 9 Nov 2020 11:18:56am
    Author:  idakil

  ==============================================================================
*/

#pragma once
#include "CircularBuffer.h"
/*
struct CompressorComponent : public Component {
    Slider threshSlider, slopeSlider, kneeSlider, attackSlider, releaseSlider;
    Label threshLabel, slopeLabel, kneeLabel, attackLabel, releaseLabel;
    SliderParameterAttachment threshAttachment, slopeAttachment, kneeAttachment, attackAttachment, releaseAttachment;

};*/

class Compressor {
public:
    Compressor();
    float compressSample(float data, float thresh, float ratio, float attack, float release, float knee);
    float interpolatePoints(float* xPoints, float* yPoints, float detectedValue);
private:
    CircularBuffer buffer;
    float tav, rms, gain;
};