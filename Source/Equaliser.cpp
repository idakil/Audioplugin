
#include "Equaliser.h"
#include "EqualiserProcessor.h"

EqBandComponent::EqBandComponent(FilterBand& band)
    : freqSlider(Slider::LinearVertical,Slider::TextBoxBelow)
    , qualSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , gainSlider(Slider::LinearVertical, Slider::TextBoxBelow)
    , freqAttachment(*band.freqParam, freqSlider)
    , qualAttachment(*band.qualParam, qualSlider)
    , gainAttachment(*band.gainParam, gainSlider)
{
    addAndMakeVisible(freqSlider);
    addAndMakeVisible(qualSlider);
    addAndMakeVisible(gainSlider);
}

void EqBandComponent::paint(juce::Graphics& g)
{
}

void EqBandComponent::resized()
{
    auto bounds = getLocalBounds();
    const int third = bounds.getWidth() / 3;

    freqSlider.setBounds(bounds.removeFromLeft(third));
    qualSlider.setBounds(bounds.removeFromLeft(third));
    gainSlider.setBounds(bounds);
}

Equaliser::Equaliser(EqualiserProcessor& p)
    : audioProcessor(p)
    , bandKnobs0(p.band0)
    , bandKnobs1(p.band1)
{
    addAndMakeVisible(bandKnobs0);
    addAndMakeVisible(bandKnobs1);
    setSize(400, 300);
}

Equaliser::~Equaliser()
{
}

