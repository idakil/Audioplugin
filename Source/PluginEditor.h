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
    windowHeight = 600,
    panelHeight = windowHeight - 30,
    labelPosition = 15,
    sliderHeightMargin = 80
};
//==============================================================================

struct DelayComponent : public Component {
    DelayComponent(DelayProcessor& processor);

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider delayLenghtSlider;
    Slider delayFeedbackSlider;
    Slider delayWetDryMixSlider;

    SliderParameterAttachment delayLenghtAttachment;
    SliderParameterAttachment delayFeedbackAttachment;
    SliderParameterAttachment delayWetDryMixAttachment;

    FlexBox delayFlexBox;
};

struct DistortionComponent : public Component {
    DistortionComponent(DistortionProcessor& processor);

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider thresholdSlider;
    Slider ratioSlider;
    Slider attackSlider;
    Slider releaseSlider;
    Slider saturationSlider;

    SliderParameterAttachment thresholdAttachment;
    SliderParameterAttachment ratioAttachment;
    SliderParameterAttachment attackAttachment;
    SliderParameterAttachment releaseAttachment;
    SliderParameterAttachment saturationAttachment;

    FlexBox distortionFlexBox;
};

struct ChorusComponent : public Component {

    ChorusComponent(ChorusProcessor& processor);

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider chorusLenghtSlider;
    Slider chorusModulationSlider;
    Slider chorusLfoSpeedSlider;
    Slider chorusWetDryMixSlider;

    SliderParameterAttachment chorusLenghtAttachment;
    SliderParameterAttachment chorusModulationAttachment;
    SliderParameterAttachment chorusLfoAttachment;
    SliderParameterAttachment chorusWetDryMixAttachment;

    FlexBox chorusFlexBox;
};

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

struct EqualiserContainer : public Component {
    EqualiserContainer(Equaliser eq0, Equaliser eq1) :
        eqBandKnobs0(eq0),
        eqBandKnobs1(eq1)
    {
        addAndMakeVisible(eqBandKnobs0);
        addAndMakeVisible(eqBandKnobs1);
        eqContainerFlex.items.add(juce::FlexItem(eqBandKnobs0).withFlex(1));
        eqContainerFlex.items.add(juce::FlexItem(eqBandKnobs1).withFlex(1));
        setSize(windowWidth, panelHeight);
        eqContainerFlex.flexDirection = juce::FlexBox::Direction::column;
        eqContainerFlex.justifyContent = juce::FlexBox::JustifyContent::center;
    }
    void paint(juce::Graphics& g) override {
    };
    void resized() override {
        eqContainerFlex.performLayout(getLocalBounds().toFloat());
    };
    EqBandComponent eqBandKnobs0, eqBandKnobs1;
    FlexBox eqContainerFlex;
};
//==============================================================================

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

struct CompressorContainer : public Component {
    CompressorContainer(Compressor comp0, Compressor comp1) :
        compressorKnobs0(comp0),
        compressorKnobs1(comp1)
    {
        addAndMakeVisible(compressorKnobs0);
        addAndMakeVisible(compressorKnobs1);
        compContainerFlex.items.add(juce::FlexItem(compressorKnobs0).withFlex(1));
        compContainerFlex.items.add(juce::FlexItem(compressorKnobs1).withFlex(1));
        setSize(windowWidth, panelHeight);
        compContainerFlex.flexDirection = juce::FlexBox::Direction::column;
        compContainerFlex.justifyContent = juce::FlexBox::JustifyContent::center;
    }
    void paint(juce::Graphics& g) override {
    };
    void resized() override {
        compContainerFlex.performLayout(getLocalBounds().toFloat());
    };
    FlexBox compContainerFlex;
    CompressorComponent compressorKnobs0, compressorKnobs1;
};
//==============================================================================

struct BitCrusherComponent : public Component {
    BitCrusherComponent(BitCrusher& bitCrusher);
    ~BitCrusherComponent() {};

    void paint(juce::Graphics& g) override;
    void resized() override;

    Slider bitReduxSlider, rateReduxSlider, noiseSlider;
    SliderParameterAttachment bitReduxAttachment, rateReduxAttachment, noiseAttachment;
    FlexBox bitCrusherFlexBox;
};
//==============================================================================

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
    EffectComponentContainer(AudiopluginAudioProcessor& p) {
        for (auto i = concertinaPanel.getNumPanels() - 1; i >= 0; --i)
            concertinaPanel.removePanel(concertinaPanel.getPanel(i));

        headers.clear();
        concertinaPanel.addPanel(0, new EqualiserContainer(p.eq0, p.eq1), true);
        concertinaPanel.addPanel(1, new CompressorContainer(p.compressor0, p.compressor1), true);
        concertinaPanel.addPanel(2, new BitCrusherComponent(p.bitCrusher), true);
        concertinaPanel.addPanel(3, new DelayComponent(p.delay), true);
        concertinaPanel.addPanel(4, new ChorusComponent(p.chorus), true);
        concertinaPanel.addPanel(5, new DistortionComponent(p.distortion), true);

        headers.add(new ConcertinaHeader("Equaliser"));
        headers.add(new ConcertinaHeader("Compressors"));
        headers.add(new ConcertinaHeader("Bit Crusher"));
        headers.add(new ConcertinaHeader("Delay"));
        headers.add(new ConcertinaHeader("Chorus"));
        headers.add(new ConcertinaHeader("Distortion"));

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
        //flexBox.items.add(juce::FlexItem(concertinaPanel).withMinHeight(10.0f).withFlex(1));

        //flexBox.flexDirection = juce::FlexBox::Direction::column;
        //flexBox.justifyContent = juce::FlexBox::JustifyContent::center;
        concertinaPanel.setSize(windowWidth, panelHeight);
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

    Viewport viewPort;

    FlexBox flexBox;

    ConcertinaPanel concertinaPanel;
    OwnedArray<ConcertinaHeader> headers;
};
//==============================================================================

struct MainView : public juce::Component {
    MainView(AudiopluginAudioProcessor& p) {
        //p.compressor0.thresh = 0.1f;
        Logger::outputDebugString(std::to_string(p.compressor0.thresh));

        addAndMakeVisible(rect);
        addMouseListener(this, true);
        setSize(windowWidth, panelHeight);
    }
    void resize() {
        rect.setBounds(20, 20, windowWidth, panelHeight);
    }
    void paint(juce::Graphics& g)
    {
        juce::Rectangle<float> area(0, 0, windowWidth, panelHeight);
        //juce::Parallelogram<float> p(area);
        //rect.setRectangle(p);
        g.setColour(juce::Colours::aqua);
        g.fillRect(area);
        setPoints(g);
    }
    void setPoints(juce::Graphics& g) {

    }
    void mouseDrag(const MouseEvent &event) {
        float midX = 300.0f/(event.getPosition().getX());
        float midY = 200.0f / (event.getPosition().getY());

        p.compressor0.changeParams(midX, midY);
        Logger::outputDebugString(std::to_string(p.compressor0.threshParam->get()));

    }
    AudiopluginAudioProcessor p;
    DrawableRectangle rect;
};
//==============================================================================

class AudiopluginAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    enum
    {
        marginTop = 80,
        gainLabelWidth = 80,
        gainSliderWidth = 300,
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
    std::unique_ptr<juce::TabbedButtonBar> buttonBar;
    juce::AudioProcessorValueTreeState& valueTreeState;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudiopluginAudioProcessorEditor)
};
