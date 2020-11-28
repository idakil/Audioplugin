#pragma once
#include <JuceHeader.h>
#include "biquad.hpp"

struct Equaliser : public juce::AudioProcessorParameter::Listener {

    template <class AudioProcessorType>
    Equaliser(AudioProcessorType& processor, juce::String bandName, int defaultType, double& fs) : samplerate(fs) {
        freqParam = new AudioParameterFloat(bandName + "freq", bandName + " Freq", 20, 20000, 200);
        qualParam = new AudioParameterFloat(bandName + "qual", bandName + " Q", 0.1, 16, 0.707);
        gainParam = new AudioParameterFloat(bandName + "gain", bandName + " Gain", -30, 30, 0);
        typeParam = new AudioParameterChoice(bandName + "type", bandName + " Type", { "Lowpass", "Peaking", }, defaultType);

        // Add the newly created parameters to the audio processor
        processor.addParameter(freqParam);
        processor.addParameter(qualParam);
        processor.addParameter(gainParam);
        processor.addParameter(typeParam);

        // register as listener
        freqParam->addListener(this);
        qualParam->addListener(this);
        gainParam->addListener(this);
        typeParam->addListener(this);
    }

    Equaliser() = delete;

    void prepare(int numChannels);
    void process(float& leftSample, float& rightSample);

    // This function is called when values change
    // We don't actually need the parameterIndex and newValue here, so commenting the names out removes "parameter unused" compiler warning
    void parameterValueChanged(int /* parameterIndex */, float /* newValue */) override;

    // This is part of the life of being a Parameter Listener, we need to implement unnecessary stuff due to library design in JUCE.
    // Just implement with empty body for now.
    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override {};

    // The actual EQ state
    // Store the biquads into the processor state. Since the filter has a state that should be
    // carried over form block to block, we can't just create a new filter in every block.
    //
    // To make this more general, we're storing the biquads into a vector and allocate the memory for them.
    std::vector<Biquad<double>> biquads;

    // Parameters that the band needs
    juce::AudioParameterFloat* freqParam, * qualParam, * gainParam;
    juce::AudioParameterChoice* typeParam;

    double& samplerate; // let's store a reference of samplerate that the AudioProcessor maintains

};

/*
struct EqBandComponent : public Component
{
    EqBandComponent(FilterBand& band);

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider freqSlider;
    Slider qualSlider;
    Slider gainSlider;
    Label freqLabel, qualLabel, gainLabel;

    SliderParameterAttachment freqAttachment;
    SliderParameterAttachment qualAttachment;
    SliderParameterAttachment gainAttachment;
};

class Equaliser : public Component, public ProcessorBase
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
    //void prepareToPlay(double sampleRate, int samplesPerBlock);
    void processBlock(AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages);
    void pluginParameters(int index, float parameter);
private:
    AudiopluginAudioProcessor& audioProcessor;
    EqBandComponent bandKnobs0;
    EqBandComponent bandKnobs1;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Equaliser)
};*/