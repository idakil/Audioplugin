#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

struct EqBandComponent : public Component
{
    EqBandComponent(FilterBand& band);

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider freqSlider;
    Slider qualSlider;
    Slider gainSlider;

    SliderParameterAttachment freqAttachment;
    SliderParameterAttachment qualAttachment;
    SliderParameterAttachment gainAttachment;
};

class Equaliser : public Component
{
public:
	Equaliser(AudiopluginAudioProcessor&);
    ~Equaliser() override;
    void paint(juce::Graphics& g) {

    };
    void resized() {
        auto bounds = getLocalBounds();
        int h = bounds.getHeight() / 2;
        bandKnobs0.setBounds(bounds.removeFromTop(h));
        bandKnobs1.setBounds(bounds);
    };
    void showEqualiser() {
        addAndMakeVisible(bandKnobs0);
        addAndMakeVisible(bandKnobs1);
        setSize(400, 300);
    }
private:
	AudiopluginAudioProcessor& audioProcessor;
    EqBandComponent bandKnobs0;
    EqBandComponent bandKnobs1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Equaliser)
};

