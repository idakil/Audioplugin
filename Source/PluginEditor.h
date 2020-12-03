/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

enum MyEnum
{
    windowWidth = 600,
    windowHeight = 600,
    panelHeight = windowHeight - 30,
    labelPosition = 15,
    sliderHeightMargin = 80
};
//==============================================================================

struct DelayComponent : public juce::Component {
    DelayComponent(DelayProcessor& processor);

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::Slider delayLenghtSlider;
    juce::Slider delayFeedbackSlider;
    juce::Slider delayWetDryMixSlider;

    juce::SliderParameterAttachment delayLenghtAttachment;
    juce::SliderParameterAttachment delayFeedbackAttachment;
    juce::SliderParameterAttachment delayWetDryMixAttachment;

    juce::FlexBox delayFlexBox;
};

struct DistortionComponent : public juce::Component {
    DistortionComponent(DistortionProcessor& processor);

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::Slider thresholdSlider;
    juce::Slider ratioSlider;
    juce::Slider attackSlider;
    juce::Slider releaseSlider;
    juce::Slider saturationSlider;

    juce::SliderParameterAttachment thresholdAttachment;
    juce::SliderParameterAttachment ratioAttachment;
    juce::SliderParameterAttachment attackAttachment;
    juce::SliderParameterAttachment releaseAttachment;
    juce::SliderParameterAttachment saturationAttachment;

    juce::FlexBox distortionFlexBox;
};

struct ChorusComponent : public juce::Component {

    ChorusComponent(ChorusProcessor& processor);

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::Slider chorusLenghtSlider;
    juce::Slider chorusModulationSlider;
    juce::Slider chorusLfoSpeedSlider;
    juce::Slider chorusWetDryMixSlider;

    juce::SliderParameterAttachment chorusLenghtAttachment;
    juce::SliderParameterAttachment chorusModulationAttachment;
    juce::SliderParameterAttachment chorusLfoAttachment;
    juce::SliderParameterAttachment chorusWetDryMixAttachment;

    juce::FlexBox chorusFlexBox;
};

struct EqBandComponent : public juce::Component
{
    EqBandComponent(Equaliser& eq);
    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::Slider freqSlider;
    juce::Slider qualSlider;
    juce::Slider gainSlider;

    juce::SliderParameterAttachment freqAttachment;
    juce::SliderParameterAttachment qualAttachment;
    juce::SliderParameterAttachment gainAttachment;

    juce::FlexBox eqFlexBox;
    juce::TextButton header;
};

struct EqualiserContainer : public juce::Component {
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
    juce::FlexBox eqContainerFlex;
};
//==============================================================================

struct CompressorComponent : public juce::Component {

    CompressorComponent(Compressor& comp);
    ~CompressorComponent() {};

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::Slider threshSlider, slopeSlider, attackSlider, releaseSlider;
    juce::Label threshLabel, slopeLabel, attackLabel, releaseLabel;
    juce::SliderParameterAttachment threshAttachment, slopeAttachment, attackAttachment, releaseAttachment;
    juce::FlexBox compressorFlexBox;
};

struct CompressorContainer : public juce::Component {
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
    juce::FlexBox compContainerFlex;
    CompressorComponent compressorKnobs0, compressorKnobs1;
};
//==============================================================================

struct BitCrusherComponent : public juce::Component {
    BitCrusherComponent(BitCrusher& bitCrusher);
    ~BitCrusherComponent() {};

    void paint(juce::Graphics& g) override;
    void resized() override;

    juce::Slider bitReduxSlider, rateReduxSlider, noiseSlider;
    juce::SliderParameterAttachment bitReduxAttachment, rateReduxAttachment, noiseAttachment;
    juce::FlexBox bitCrusherFlexBox;
};
//==============================================================================

class ConcertinaHeader : public juce::Component,
    public juce::ChangeBroadcaster
{
public:
    ConcertinaHeader(juce::String n)
        : juce::Component(n), name(n)
    {
        //panelIcon = Icon(iconPath, Colours::white);
        nameLabel.setText(name, juce::dontSendNotification);
        nameLabel.setJustificationType(juce::Justification::centredLeft);
        nameLabel.setInterceptsMouseClicks(false, false);
        nameLabel.setColour(juce::Label::textColourId, juce::Colours::white);

        addAndMakeVisible(nameLabel);
    }

    void resized() override
    {
        auto b = getLocalBounds().toFloat();

        iconBounds = b.removeFromLeft(b.getHeight()).reduced(7, 7);
        arrowBounds = b.removeFromRight(b.getHeight());
        nameLabel.setBounds(b.toNearestInt());
    }

    void paint(juce::Graphics& g) override
    {
        //g.setColour(findColour(defaultButtonBackgroundColourId));
        g.fillRoundedRectangle(getLocalBounds().reduced(2, 3).toFloat(), 2.0f);

        g.setColour(juce::Colours::white);
        /* g.fillPath(arrowPath = ProjucerLookAndFeel::getArrowPath(arrowBounds,
             getParentComponent()->getBoundsInParent().getY() == yPosition ? 2 : 0,
             true, Justification::centred));*/

    }

    void mouseUp(const juce::MouseEvent& e) override
    {
        if (!e.mouseWasDraggedSinceMouseDown())
            sendChangeMessage();
    }

    int direction = 0;
    int yPosition = 0;

private:
    juce::String name;
    juce::Label nameLabel;

    juce::Path iconPath;

    juce::Rectangle<float> arrowBounds, iconBounds;
    juce::Path arrowPath;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ConcertinaHeader)
};

struct EffectComponentContainer : public juce::Component, private juce::ChangeListener {
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

    void changeListenerCallback(juce::ChangeBroadcaster* source) override
    {
        if (auto* header = dynamic_cast<ConcertinaHeader*> (source))
        {
            auto index = headers.indexOf(header);
            concertinaPanel.expandPanelFully(concertinaPanel.getPanel(index), true);
        }
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        flexBox.performLayout(getLocalBounds().toFloat());
    }

    juce::Viewport viewPort;

    juce::FlexBox flexBox;

    juce::ConcertinaPanel concertinaPanel;
    juce::OwnedArray<ConcertinaHeader> headers;
};
//==============================================================================

struct MainView : public juce::AnimatedAppComponent, juce::Slider::Listener {

    MainView(AudiopluginAudioProcessor& p) 
    : offsetSlider(juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight)
    , speedSlider(juce::Slider::LinearHorizontal, juce::Slider::TextBoxRight)
    {
        //p.compressor0.thresh = 0.1f;
        juce::Logger::outputDebugString(std::to_string(p.compressor0.thresh));

        addAndMakeVisible(offsetSlider);
        offsetSlider.addListener(this);
        offsetSlider.setRange(0.01, 1.0, 0.01);
        offsetSlider.setValue(circleOffset);

        addAndMakeVisible(speedSlider);
        speedSlider.addListener(this);
        speedSlider.setRange(0.01, 1.0, 0.01);
        speedSlider.setValue(circleSpeed);

        setFramesPerSecond(60);

        addMouseListener(this, true);
    }

    void update() override
    {

    }

    void sliderValueChanged(juce::Slider *slider)
    {
        circleOffset = offsetSlider.getValue();
        circleSpeed = speedSlider.getValue();
    }

    void paint(juce::Graphics& g) override
    {
        g.fillAll(juce::Colours::goldenrod);

        auto arcLenght = 15;
        int radius = 15 + getHeight() * 0.01;
        
        juce::Path arc1;
        juce::Path arc2;

        for (auto i = 0; i < arcLenght; ++i)
        {

            juce::Point<float> p1(circleX + 1.0f * radius * std::sin(getFrameCounter() * circleSpeed + i * circleOffset),
                circleY + 1.0f * radius * std::cos(getFrameCounter() * circleSpeed + i * circleOffset));

            juce::Point<float> p2(circleX + 1.0f * radius * std::sin(juce::MathConstants<double>::pi + getFrameCounter() * circleSpeed + i * circleOffset),
                circleY + 1.0f * radius * std::cos(juce::MathConstants<double>::pi + getFrameCounter() * circleSpeed + i * circleOffset));


            if (i == 0)
            {
                arc1.startNewSubPath(p1);
                arc2.startNewSubPath(p2);
            }
            else
            {
                arc1.lineTo(p1);
                arc2.lineTo(p2);
            }
                
        }

        g.setColour(juce::Colours::deeppink);
        g.strokePath(arc1, juce::PathStrokeType(4.0f));
        g.strokePath(arc2, juce::PathStrokeType(4.0f));

        g.setColour(juce::Colours::white);
        g.drawLine(0, circleY, getWidth(), circleY);
        g.drawLine(circleX, 0, circleX, getHeight());
    }

    void resized() override
    {
        offsetSlider.setBounds(0, getHeight() - 100, getWidth(), 50);
        speedSlider.setBounds(0, getHeight() - 50, getWidth(), 50);
    }

    void mouseDrag(const juce::MouseEvent& event) override
    {
        /*
        float midX = 300.0f / (event.getPosition().getX());
        float midY = 200.0f / (event.getPosition().getY());

        p.compressor0.changeParams(midX, midY);
        Logger::outputDebugString(std::to_string(p.compressor0.threshParam->get()));
        */

        circleX = event.getPosition().getX();
        circleY = event.getPosition().getY();

    }

    AudiopluginAudioProcessor p;

    float circleX = 50.0f;
    float circleY = 50.0f;

    float circleOffset = 0.16f;
    float circleSpeed = 0.04f;

    juce::Slider offsetSlider;
    juce::Slider speedSlider;
};

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
