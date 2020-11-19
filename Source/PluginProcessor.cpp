/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"



//==============================================================================
AudiopluginAudioProcessor::AudiopluginAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    )
    , parameters(*this, nullptr, Identifier("params"), createParameterLayout())
    , band0(*this, "Band 0", 1000,  0, samplerate) // call the constructors of FilterBands on initialisation
    , band1(*this, "Band 1", 4000,  1, samplerate)
    #endif
{
}

AudiopluginAudioProcessor::~AudiopluginAudioProcessor()
{
}



juce::AudioProcessorValueTreeState::ParameterLayout AudiopluginAudioProcessor::createParameterLayout()
{
    // Here we can programatically add parameters to the parameter layout.
    juce::AudioProcessorValueTreeState::ParameterLayout params;

    // ID, name, min, max, default
    params.add(std::make_unique<juce::AudioParameterFloat>("gain", "Gain Name", 0.0f, 1.0f, 0.5f));
    params.add(std::make_unique<AudioParameterFloat>(
        "thresh",
        "Threshold",
        NormalisableRange<float>(-60.0f, 20.0f, 0.01f),
        10.0f));
    params.add(std::make_unique<AudioParameterFloat>(
        "ratio",
        "Ratio",
        NormalisableRange<float>(1.0f, 20.0f, 0.01f),
        2.0f));
    params.add(std::make_unique<AudioParameterFloat>(
        "knee",
        "KneeWidth",
        NormalisableRange<float>(0.0f, 24.0f, 0.01f),
        0.0f));
    params.add(std::make_unique<AudioParameterFloat>(
        "attack",
        "Attack",
        NormalisableRange<float>(0.01f, 500.0, 0.01f),
        100.0f));
    params.add(std::make_unique<AudioParameterFloat>(
        "release",
        "Release",
        NormalisableRange<float>(0.01f, 2000.0f, 0.01f),
        500.0f));
    return params;
}

//==============================================================================
const juce::String AudiopluginAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AudiopluginAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AudiopluginAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AudiopluginAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AudiopluginAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AudiopluginAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int AudiopluginAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AudiopluginAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AudiopluginAudioProcessor::getProgramName (int index)
{
    return {};
}

void AudiopluginAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void AudiopluginAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    /*
    // store this for later use
    this->samplerate = sampleRate;
    int numChannels = getTotalNumInputChannels();
    band0.prepare(numChannels);
    band1.prepare(numChannels);

    for (int channel = 0; channel < getNumOutputChannels(); channel++) {
        allCompressors.add(Compressor());
    }
    threshParam = parameters.getRawParameterValue("thresh");
    slopeParam = parameters.getRawParameterValue("ratio");
    kneeParam = parameters.getRawParameterValue("knee");
    attackParam = parameters.getRawParameterValue("attack");
    releaseParam = parameters.getRawParameterValue("release");*/
}

void AudiopluginAudioProcessor::releaseResources()
{
    mainProcessor->releaseResources();
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

/*
* void CompressorAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    float at = 1 - std::pow(MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (*attackParam / 1000.0f));
    float rt = 1 - std::pow(MathConstants<float>::euler, ((1 / getSampleRate()) * -2.2f) / (*releaseParam / 1000.0f));

    for (int i = 0; i < buffer.getNumSamples(); i++) {
        for (int channel = 0; channel < getTotalNumOutputChannels(); channel++) {
            auto* data = buffer.getWritePointer(channel);
            Compressor* comp = &allCompressors.getReference(channel);
            data[i] = comp->compressSample(data[i], *threshParam, *slopeParam, at, rt, *kneeParam);
        }
    }
}
*/
void AudiopluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamples);


    mainProcessor->processBlock(buffer, midiMessages);
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    /*for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        for (int i = 0; i < numSamples; ++i)
        {
            // for easier reading, copy the sample from io buffer
            // this should be optimised by the compiler
            auto sample = channelData[i];

            // Replace sample with all bands, all bands run in series
            // There's an array of Biquad objects inside each band, with the name biquad.
            // A specific Biquad object per channel is then accessed with the [ch], and performFilter is called
            // It takes a sample as an input, and returns a sample, that we replace our "sample" with.
            sample = band0.biquads[channel].performFilter(sample);
            sample = band1.biquads[channel].performFilter(sample);

            // write back to io buffer
            channelData[i] = sample;
        }
        // ..do something to the data...
        
    }*/
}

//==============================================================================
bool AudiopluginAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AudiopluginAudioProcessor::createEditor()
{
    return new AudiopluginAudioProcessorEditor (*this, parameters);
}

void AudiopluginAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void AudiopluginAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));
}

bool AudiopluginAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    // Accept mono-mono and stereo-stereo only
    if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::mono()
        && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::mono())
    {
        return true;
    }
    // Accept mono-mono and stereo-stereo only
    if (layouts.getMainInputChannelSet() == juce::AudioChannelSet::stereo()
        && layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo())
    {
        return true;
    }

    return false;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudiopluginAudioProcessor();
}
