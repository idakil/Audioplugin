/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Presets.h"

enum MyEnum
{
    windowWidth = 600,
    windowHeight = 800,
    panelHeight = windowHeight - 30,
    labelPosition = 15,
    sliderHeightMargin = 80,
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
    juce::Slider saturationSlider;

    juce::SliderParameterAttachment thresholdAttachment;
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
    juce::Slider wetDrySlider;

    juce::SliderParameterAttachment freqAttachment;
    juce::SliderParameterAttachment qualAttachment;
    juce::SliderParameterAttachment gainAttachment;
    juce::SliderParameterAttachment wetDryAttachment;

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

        //setSize(windowWidth, panelHeight);

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

    juce::Slider threshSlider, slopeSlider, attackSlider, releaseSlider, gainSlider, wetDrySlider;
    juce::Label threshLabel, slopeLabel, attackLabel, releaseLabel, gainLabel;
    juce::SliderParameterAttachment threshAttachment, slopeAttachment, attackAttachment, releaseAttachment, gainAttachment, wetDryAttachment;
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
        g.setColour(juce::Colours::black);
        g.fillRect(compressorKnobs0.getX(), compressorKnobs0.getHeight() + 5, compressorKnobs0.getWidth(), 2);
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

    juce::Slider bitReduxSlider, rateReduxSlider, noiseSlider, wetDrySlider;
    juce::SliderParameterAttachment bitReduxAttachment, rateReduxAttachment, noiseAttachment, wetDryAttachment;
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
        nameLabel.setText(name, juce::dontSendNotification);
        nameLabel.setJustificationType(juce::Justification::centredLeft);
        nameLabel.setInterceptsMouseClicks(false, false);
        nameLabel.setColour(juce::Label::textColourId, juce::Colours::white);

        addAndMakeVisible(nameLabel);
    }

    void resized() override
    {
        auto b = getLocalBounds().toFloat();
        nameLabel.setBounds(b.toNearestInt());
    }

    void paint(juce::Graphics& g) override
    {
        g.setColour(juce::Colour(0xff2b123b));
        g.fillRoundedRectangle(getLocalBounds().reduced(2, 3).toFloat(), 2.0f);

        g.setColour(juce::Colours::white);
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
 
        addAndMakeVisible(concertinaPanel);
        flexBox.items.add(juce::FlexItem(concertinaPanel).withFlex(1));
        //concertinaPanel.setSize(windowWidth, panelHeight);
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
        g.fillAll(juce::Colour(0xffe7d1ff).withMultipliedBrightness(0.8));
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
    , pro(p)
    {
        //addAndMakeVisible(offsetSlider);
        //offsetSlider.addListener(this);
        //offsetSlider.setRange(0.01, 1.0, 0.01);
        //offsetSlider.setValue(circleOffset);

        //addAndMakeVisible(speedSlider);
        //speedSlider.addListener(this);
        //speedSlider.setRange(0.01, 1.0, 0.01);
        //speedSlider.setValue(circleSpeed);

        setFramesPerSecond(60);

        addMouseListener(this, true);

    }

    void update() override
    {

    }

    void sliderValueChanged(juce::Slider *slider) override
    {
        circleOffset = offsetSlider.getValue();
        circleSpeed = speedSlider.getValue();
    }

    void paint(juce::Graphics& g) override
    {
        g.drawImageWithin(myImage, 0, 0, getParentWidth(), getParentHeight(), juce::RectanglePlacement::stretchToFit, false);
        //g.fillAll(juce::Colours::goldenrod);

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
        // 1. Vibes only 2. The Garage 3. Spacy 4. Bitter
        g.setColour(juce::Colours::white);
        g.setFont(18.0f);
        g.drawText("Vibes only", 10, 5, 100, 20, juce::Justification::topLeft); //topleft
        g.drawText("The Garage", getWidth()-110, 5, 100, 20, juce::Justification::topRight); //topright
        g.drawText("Spacy", 10, getHeight() - 25, 100, 20, juce::Justification::topLeft);
        g.drawText("Bitter", getWidth()-110, getHeight() - 25, 100, 20, juce::Justification::topRight);

        //g.drawText(std::to_string(leftRatio), getWidth() / 2, getHeight() / 2, 100, 200, juce::Justification::centred);

    }

    void resized() override
    {

        offsetSlider.setBounds(0, getHeight() - 100, getWidth(), 50);
        speedSlider.setBounds(0, getHeight() - 50, getWidth(), 50);
    }

    void mouseDrag(const juce::MouseEvent& event) override
    {
        circleX = event.getPosition().getX();
        circleY = event.getPosition().getY();

        //bitr.beginGesture();

        //if (circleX < vibesArea.getBottomRight().getX() && circleY< vibesArea.getBottomRight().getY()) {
        //    bitr.setValue(4.5);
        //}

        rightRatio = (float) event.getPosition().getX() / (float) getWidth();
        leftRatio = 1.0f - rightRatio;

        bottomRatio = (float) event.getPosition().getY() / (float) getHeight();
        topRatio = 1.0f - bottomRatio;

        *pro.eq0.freqParam = leftRatio * topRatio * 6000.0f + rightRatio * topRatio * 1960.0f + leftRatio * bottomRatio * 5000.0f + rightRatio * bottomRatio * 0.0f;
        *pro.eq0.gainParam = leftRatio * topRatio * 0.0f + rightRatio * topRatio * 8.0f + leftRatio * bottomRatio * 30.0f + rightRatio * bottomRatio * 0.0f;
        *pro.eq0.qualParam = leftRatio * topRatio * 0.1f + rightRatio * topRatio * 0.43f + leftRatio * bottomRatio * 0.1f + rightRatio * bottomRatio * 0.0f;
        *pro.eq0.wetDryParam = leftRatio * topRatio * 0.0f + rightRatio * topRatio * 1.0f + leftRatio * bottomRatio * 1.0f + rightRatio * bottomRatio * 0.0f;

        *pro.eq1.freqParam = leftRatio * topRatio * 6000.0f + rightRatio * topRatio * 8320.0f + leftRatio * bottomRatio * 8320.0f + rightRatio * bottomRatio * 0.0f;
        *pro.eq1.gainParam = leftRatio * topRatio * 0.0f + rightRatio * topRatio * 0.0f + leftRatio * bottomRatio * 0.0f + rightRatio * bottomRatio * 0.0f;
        *pro.eq1.qualParam = leftRatio * topRatio * 0.1f + rightRatio * topRatio * 1.6f + leftRatio * bottomRatio * 1.6f + rightRatio * bottomRatio * 0.0f;
        *pro.eq1.wetDryParam = leftRatio * topRatio * 1.0f + rightRatio * topRatio * 1.0f + leftRatio * bottomRatio * 0.0f + rightRatio * bottomRatio * 0.0f;

        *pro.compressor0.attackParam = leftRatio * topRatio * 10.0f + rightRatio * topRatio * 6.0f + leftRatio * bottomRatio * 6.0f + rightRatio * bottomRatio * 0.0f;
        *pro.compressor0.ratioParam = leftRatio * topRatio * 6.0f + rightRatio * topRatio * 7.0f + leftRatio * bottomRatio * 7.0f + rightRatio * bottomRatio * 0.0f;
        *pro.compressor0.releaseParam = leftRatio * topRatio * 200.0f + rightRatio * topRatio * 30.0f + leftRatio * bottomRatio * 30.0f + rightRatio * bottomRatio * 0.0f;
        *pro.compressor0.threshParam = leftRatio * topRatio * -20.0f + rightRatio * topRatio * -12.0f + leftRatio * bottomRatio * -12.0f + rightRatio * bottomRatio * 0.0f;

        *pro.compressor1.attackParam = leftRatio * topRatio * 20.0f + rightRatio * topRatio * 30.0f + leftRatio * bottomRatio * 30.0f + rightRatio * bottomRatio * 0.0f;
        *pro.compressor1.ratioParam = leftRatio * topRatio * 2.0f + rightRatio * topRatio * 12.0f + leftRatio * bottomRatio * 12.0f + rightRatio * bottomRatio * 0.0f;
        *pro.compressor1.releaseParam = leftRatio * topRatio * 200.0f + rightRatio * topRatio * 200.0f + leftRatio * bottomRatio * 200.0f + rightRatio * bottomRatio * 0.0f;
        *pro.compressor1.threshParam = leftRatio * topRatio * -12.0f + rightRatio * topRatio * -10.0f + leftRatio * bottomRatio * -10.0f + rightRatio * bottomRatio * 0.0f;

        *pro.delay.feedbackParam = leftRatio * topRatio * 0.2f + rightRatio * topRatio * 0.2f + leftRatio * bottomRatio * 0.2f + rightRatio * bottomRatio * 0.48f;
        *pro.delay.wetDryMixParam = leftRatio * topRatio * 40.0f + rightRatio * topRatio * 20.0f + leftRatio * bottomRatio * 0.0f + rightRatio * bottomRatio * 52.0f; 
        *pro.delay.lenghtParam = leftRatio * topRatio * 15.0f + rightRatio * topRatio * 3.0f + leftRatio * bottomRatio * 3.0f + rightRatio * bottomRatio * 43.0f;

        *pro.chorus.lenghtParam = leftRatio * topRatio * 0.04f + rightRatio * topRatio * 0.01f + leftRatio * bottomRatio * 0.01f + rightRatio * bottomRatio * 0.32f;
        *pro.chorus.lfoSpeedParam = leftRatio * topRatio * 3.0f + rightRatio * topRatio * 0.0f + leftRatio * bottomRatio * 0.0f + rightRatio * bottomRatio * 1.22f;
        *pro.chorus.modAmountParam = leftRatio * topRatio * 0.2f + rightRatio * topRatio * 0.0f + leftRatio * bottomRatio * 0.0f + rightRatio * bottomRatio * 2.58f;
        *pro.chorus.wetDryMixParam = leftRatio * topRatio * 0.6f + rightRatio * topRatio * 0.5f + leftRatio * bottomRatio * 0.4f + rightRatio * bottomRatio * 0.25f;

        *pro.distortion.saturationParam = leftRatio * topRatio * 5.0f + rightRatio * topRatio * 13.0f + leftRatio * bottomRatio * 4.0f + rightRatio * bottomRatio * 4.0f;
        *pro.distortion.thresholdParam = leftRatio* topRatio * 0.0f + rightRatio * topRatio * 0.0f + leftRatio * bottomRatio * -20.0f + rightRatio * bottomRatio * -16.0f;

        *pro.bitCrusher.bitReduxParam = leftRatio * topRatio * 32.0f + rightRatio * topRatio * 32.0f + leftRatio * bottomRatio * 8.0f + rightRatio * bottomRatio * 32.0f;
        *pro.bitCrusher.noiseParam = leftRatio * topRatio * 0.0f + rightRatio * topRatio * 0.0f + leftRatio * bottomRatio * 30.0f + rightRatio * bottomRatio * 0.0f;
        *pro.bitCrusher.rateReduxParam = leftRatio * topRatio * 1.0f + rightRatio * topRatio * 1.0f + leftRatio * bottomRatio * 40.0f + rightRatio * bottomRatio * 1.0f;
        *pro.bitCrusher.wetDryParam = leftRatio * topRatio * 0.5f + rightRatio * topRatio * 0.5f + leftRatio * bottomRatio * 0.5f + rightRatio * bottomRatio * 0.5f;
    }
        

    float circleX = 50.0f;
    float circleY = 50.0f;

    float circleOffset = 0.16f;
    float circleSpeed = 0.04f;
    juce::Slider offsetSlider;
    juce::Slider speedSlider;

    float rightRatio = 0.0f;
    float leftRatio = 0.0f;
    float bottomRatio = 0.0f;
    float topRatio = 0.0f;

    AudiopluginAudioProcessor& pro;

    juce::Image myImage = juce::ImageFileFormat::loadFrom(BinaryData::pluginBackground_png, BinaryData::pluginBackground_pngSize);

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
