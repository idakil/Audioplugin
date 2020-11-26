/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;


enum MyEnum
{
    windowWidth = 600,
    windowHeight = 400
};
//==============================================================================
/**
*/
struct EqBandComponent : public Component
{
    EqBandComponent(Equaliser& eq);
    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider freqSlider;
    Slider qualSlider;
    Slider gainSlider;
    SliderParameterAttachment freqAttachment;
    SliderParameterAttachment qualAttachment;
    SliderParameterAttachment gainAttachment;
    FlexBox eqFlexBox;
    TextButton header;
};

struct CompressorComponent : public Component {

    CompressorComponent(Compressor& comp);
    ~CompressorComponent() {};

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider threshSlider, slopeSlider, attackSlider, releaseSlider;
    Label threshLabel, slopeLabel, attackLabel, releaseLabel;
    SliderParameterAttachment threshAttachment, slopeAttachment, attackAttachment, releaseAttachment;
    FlexBox compressorFlexBox;
};

struct BitCrusherComponent : public Component {
    BitCrusherComponent(BitCrusher& bitCrusher);
    ~BitCrusherComponent() {};

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider bitReduxSlider, rateReduxSlider, noiseSlider;
    SliderParameterAttachment bitReduxAttachment, rateReduxAttachment, noiseAttachment;
    FlexBox bitCrusherFlexBox;
};

class ConcertinaHeader : public Component,
    public ChangeBroadcaster
{
public:
    ConcertinaHeader(String n)
        : Component(n), name(n)
    {
        //panelIcon = Icon(iconPath, Colours::white);

        nameLabel.setText(name, dontSendNotification);
        nameLabel.setJustificationType(Justification::centredLeft);
        nameLabel.setInterceptsMouseClicks(false, false);
        nameLabel.setColour(Label::textColourId, Colours::white);

        addAndMakeVisible(nameLabel);
    }

    void resized() override
    {
        auto b = getLocalBounds().toFloat();

        iconBounds = b.removeFromLeft(b.getHeight()).reduced(7, 7);
        arrowBounds = b.removeFromRight(b.getHeight());
        nameLabel.setBounds(b.toNearestInt());
    }

    void paint(Graphics& g) override
    {
        //g.setColour(findColour(defaultButtonBackgroundColourId));
        g.fillRoundedRectangle(getLocalBounds().reduced(2, 3).toFloat(), 2.0f);

        g.setColour(Colours::white);
        /* g.fillPath(arrowPath = ProjucerLookAndFeel::getArrowPath(arrowBounds,
             getParentComponent()->getBoundsInParent().getY() == yPosition ? 2 : 0,
             true, Justification::centred));*/

    }

    void mouseUp(const MouseEvent& e) override
    {
        if (!e.mouseWasDraggedSinceMouseDown())
            sendChangeMessage();
    }

    int direction = 0;
    int yPosition = 0;

private:
    String name;
    Label nameLabel;

    Path iconPath;

    Rectangle<float> arrowBounds, iconBounds;
    Path arrowPath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConcertinaHeader)
};

struct EffectComponentContainer : public juce::Component, private ChangeListener {
    EffectComponentContainer(AudiopluginAudioProcessor& p) :
        compressorKnobs0(p.compressor0),
        compressorKnobs1(p.compressor1),
        eqKnobs0(p.eq0),
        eqKnobs1(p.eq1),
        bitCrusherKnobs(p.bitCrusher)
    {

        for (auto i = concertinaPanel.getNumPanels() - 1; i >= 0; --i)
            concertinaPanel.removePanel(concertinaPanel.getPanel(i));

        headers.clear();
        concertinaPanel.addPanel(0, new EqBandComponent(p.eq0), true);
        concertinaPanel.addPanel(1, new CompressorComponent(p.compressor0), true);
        concertinaPanel.addPanel(2, new BitCrusherComponent(p.bitCrusher), true);

        headers.add(new ConcertinaHeader("Equaliser"));
        headers.add(new ConcertinaHeader("Compressors"));
        headers.add(new ConcertinaHeader("Bit Crusher"));

        for (int i = 0; i < concertinaPanel.getNumPanels(); ++i)
        {
            auto* p = concertinaPanel.getPanel(i);
            auto* h = headers.getUnchecked(i);
            p->addMouseListener(this, true);

            h->addChangeListener(this);
            h->yPosition = i * 30;

            concertinaPanel.setCustomPanelHeader(p, h, false);
            concertinaPanel.setPanelHeaderSize(p, 30);
        }
       /* addAndMakeVisible(concertinaPanel);
        flexBox.items.add(juce::FlexItem(concertinaPanel).withMinHeight(10.0f).withFlex(1));

        addAndMakeVisible(compressorKnobs0);
        flexBox.items.add(juce::FlexItem(compressorKnobs0).withMinHeight(10.0f).withFlex(1));

        addAndMakeVisible(compressorKnobs1);
        flexBox.items.add(juce::FlexItem(compressorKnobs1).withMinHeight(10.0f).withFlex(1));

        addAndMakeVisible(eqKnobs0);
        flexBox.items.add(juce::FlexItem(eqKnobs0).withMinHeight(10.0f).withFlex(1));

        addAndMakeVisible(eqKnobs1);
        flexBox.items.add(juce::FlexItem(eqKnobs1).withMinHeight(10.0f).withFlex(1));

        addAndMakeVisible(bitCrusherKnobs);
        flexBox.items.add(juce::FlexItem(bitCrusherKnobs).withMinHeight(10.0f).withFlex(1));*/
        addAndMakeVisible(concertinaPanel);
        flexBox.items.add(juce::FlexItem(concertinaPanel).withMinHeight(10.0f).withFlex(1));

        flexBox.flexDirection = juce::FlexBox::Direction::column;
        flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
        setSize(600, 400);
    }

    void changeListenerCallback(ChangeBroadcaster* source) override
    {
        if (auto* header = dynamic_cast<ConcertinaHeader*> (source))
        {
            auto index = headers.indexOf(header);
            concertinaPanel.expandPanelFully(concertinaPanel.getPanel(index), true);
        }
    }

    void paint(juce::Graphics& g) {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    void resized() {
        flexBox.performLayout(getLocalBounds().toFloat());
    }

    FlexBox flexBox;
    CompressorComponent compressorKnobs0;
    CompressorComponent compressorKnobs1;
    BitCrusherComponent bitCrusherKnobs;
    EqBandComponent eqKnobs0;
    EqBandComponent eqKnobs1;

    ConcertinaPanel concertinaPanel;
    OwnedArray<ConcertinaHeader> headers;
};

struct MainView : public juce::Component {
    MainView(AudiopluginAudioProcessor& p) {
        //p.compressor0.thresh = 0.1f;

        addAndMakeVisible(rect);
        addMouseListener(this, true);
        setSize(600, 400);
    }
    void resize() {
        rect.setBounds(20, 20, 500, 300);
    }
    void paint(juce::Graphics& g)
    {
        juce::Rectangle<float> area(10, 10, 400, 400);
        juce::Parallelogram<float> p(area);
        rect.setRectangle(p);
        g.setColour(juce::Colours::aqua);
    }
    void mouseDrag(const MouseEvent &event) {
    

        Logger::outputDebugString(std::to_string(event.getMouseDownX()));
    }
    DrawableRectangle rect;
};

class AudiopluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    enum
    {
        marginTop = 80,
        gainLabelWidth = 80,
        gainSliderWidth = 300,
        windowWidth = 400,
    };

    typedef juce::AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
    AudiopluginAudioProcessorEditor(AudiopluginAudioProcessor& p, juce::AudioProcessorValueTreeState& vts);
    ~AudiopluginAudioProcessorEditor() override;
    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;


private:
    AudiopluginAudioProcessor& audioProcessor;
    EffectComponentContainer container;
    MainView main;
    std::unique_ptr<juce::TabbedComponent> tabbedComponent;
    
    juce::AudioProcessorValueTreeState& valueTreeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudiopluginAudioProcessorEditor)
};
