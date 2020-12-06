/*
  ==============================================================================

    ParameterChanger.h
    Created: 6 Dec 2020 1:13:24am
    Author:  idakil

  ==============================================================================
*/


#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"

struct VibesPreset {
    VibesPreset() {};
    float peakFreq = 6000.0f, peakQual = 0.1f, peakGain = 0.0f, peakDryWet = 0.0f; //eq0
    float lowFreq = 6000.0f, lowQual = 0.1f, lowGain = 0.0f, lowDryWet = 1.0f; //eq1
    float comp0Thresh = -20.0f, comp0Ratio = 6.0f, comp0attack = 10.0f, comp0release = 200.0f; //comp0
    float comp1Thresh = -12.0f, comp1Ratio = 2.0f, comp1attack = 20.0f, comp1release = 200.0f;  //comp1
    float delayLength = 0.15f, delayFeedback = 0.2f, delayDryWet = 0.4f;
    float chorusLength = 0.04f, chorusMod = 0.2f, chorusLFO = 3.0f, chorusDryWet = 0.4f;
    float distSaturation = 5.0f, distThresh = 0.0f;
    float bitRedux = 32.0f, rateRedux = 1.0f, bitRnoise = 0.0f, bitRWetDry = 0.0f;
};

struct GaragePreset {
    GaragePreset() {};
    float lowFreq = 8320.0f, lowQual = 1.60f, lowGain = 0.0f, lowDryWet = 1.0f; //eq1
    float peakFreq = 1960.0f, peakQual = 0.43f, peakGain = 8.0f, peakDryWet = 1.0f; //eq0
    float comp0Thresh = -12.0f, comp0Ratio = 7.0f, comp0attack = 6.0f, comp0release = 30.0f; //comp0
    float comp1Thresh = -10.0f, comp1Ratio = 12.0f, comp1attack = 30.0f, comp1release = 200.0f;  //comp1
    float delayLength = 0.03f, delayFeedback = 0.2f, delayDryWet = 0.2f;
    float chorusLength = 0.01f, chorusMod = 0.0f, chorusLFO = 0.0f, chorusDryWet = 0.0f;
    float distSaturation = 13.0f, distThresh = 0.0f;
    float bitRedux = 32.0f, rateRedux = 1.0f, bitRnoise = 0.0f, bitRWetDry = 0.0f;
};

struct BitterPreset {
    BitterPreset() {};
    float lowFreq = 8320.0f, lowQual = 1.60f, lowGain = 0.0f, lowDryWet = 0.0f; //eq1
    float peakFreq = 5000.0f, peakQual = 0.1f, peakGain = 30.0f, peakDryWet = 1.0f; //eq0
    float comp0Thresh = -12.0f, comp0Ratio = 7.0f, comp0attack = 6.0f, comp0release = 30.0f, comp0WetDry = 0.0f; //comp0
    float comp1Thresh = -10.0f, comp1Ratio = 12.0f, comp1attack = 30.0f, comp1release = 200.0f, comp1WetDry = 0.0f;  //comp1
    float delayLength = 0.03f, delayFeedback = 0.2f, delayDryWet = 0.0f;
    float chorusLength = 0.01f, chorusMod = 0.0f, chorusLFO = 0.0f, chorusDryWet = 0.0f;
    float distSaturation = 4.0f, distThresh = -20.0f;
    float bitRedux = 8.0f, rateRedux = 40.0f, bitRnoise = 30.0f, bitRWetDry = 0.0f;
};



template<typename ValueType>
class ParameterAttachment : private juce::AudioProcessorParameter::Listener,
    private juce::AsyncUpdater
{
public:
    ParameterAttachment()
    {
    }

    virtual ~ParameterAttachment() override
    {
        detachFromParameter();
    }

    /**
     Thread safe way to read the current value.
     */
    ValueType getValue() const
    {
        return value.load();
    }

    /**
     Thread safe way to read the normalised value of the current value.
     */
    ValueType getNormalisedValue() const
    {
        if (parameter)
            return parameter->getNormalisableRange().convertTo0to1(value.load());

        return value.load();
    }

    /**
     Set the value from a not normalised range.min..range.max value.
     */
    void setValue(ValueType newValue)
    {
        if (parameter)
            parameter->setValueNotifyingHost(parameter->getNormalisableRange().convertTo0to1(newValue));
        else
            parameterValueChanged(0, juce::jlimit(0.0f, 1.0f, newValue));
    }

    /**
     Set the value from a normalised 0..1 value.
     */
    void setNormalisedValue(ValueType newValue)
    {
        if (parameter)
            parameter->setValueNotifyingHost(newValue);
        else
            parameterValueChanged(0, juce::jlimit(0.0f, 1.0f, newValue));
    }

    /**
     Make this value attached to the parameter with the supplied parameterID.
     */
    void attachToParameter(juce::RangedAudioParameter* parameterToUse)
    {
        detachFromParameter();

        if (parameterToUse)
        {
            parameter = parameterToUse;

            initialUpdate();

            parameter->addListener(this);
        }
    }

    void detachFromParameter()
    {
        if (parameter)
            parameter->removeListener(this);
    }

    /**
     Make sure to call this before you send changes (e.g. from mouseDown of your UI widget),
     otherwise the hosts automation will battle with your value changes.
     */
    void beginGesture()
    {
        if (parameter)
            parameter->beginChangeGesture();
    }

    /**
     Make sure to call this after finishing your changes (e.g. from mouseDown of your UI widget),
     this way the automation can take back control (like e.g. latch mode).
     */
    void endGesture()
    {
        if (parameter)
            parameter->endChangeGesture();
    }

    void parameterValueChanged(int parameterIndex, float newValue) override
    {
        juce::ignoreUnused(parameterIndex);
        if (parameter)
            value.store(ValueType(parameter->convertFrom0to1(newValue)));
        else
            value.store(ValueType(newValue));

        if (onParameterChanged)
            onParameterChanged();

        if (onParameterChangedAsync)
            triggerAsyncUpdate();
    }

    void parameterGestureChanged(int parameterIndex, bool gestureIsStarting) override
    {
        juce::ignoreUnused(parameterIndex); juce::ignoreUnused(gestureIsStarting);
    }

    void handleAsyncUpdate() override
    {
        if (onParameterChangedAsync)
            onParameterChangedAsync();
    }

    /**
     Set this lambda to be called from whatever thread is updating the parameter
     */
    std::function<void()> onParameterChanged;

    /**
     Set this lambda to be called from the message thread via AsyncUpdater
     */
    std::function<void()> onParameterChangedAsync;

private:

    void initialUpdate()
    {
        if (parameter)
            value.store(ValueType(parameter->convertFrom0to1(parameter->getValue())));
        else
            value.store(ValueType());

        if (onParameterChanged)
            onParameterChanged();
    }

    juce::RangedAudioParameter* parameter = nullptr;
    std::atomic<ValueType>              value{ ValueType() };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ParameterAttachment)
};
