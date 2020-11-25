
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
    , eq0(*this, "eq0", 0, samplerate) // call the constructors of FilterBands on initialisation
    , eq1(*this, "eq1", 1, samplerate)
    , compressor(*this, samplerate)
    , bitCrusher(*this, samplerate)
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
    this->samplerate = sampleRate;
    int numChannels = getTotalNumInputChannels();

    //eq0.prepare(numChannels);
    //eq1.prepare(numChannels);
    compressor.prepare(samplesPerBlock);
    bitCrusher.prepare();
    /*
    for (int channel = 0; channel < getNumOutputChannels(); channel++) {
        allCompressors.add(Compressor());
    }*/

}

void AudiopluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}


void AudiopluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamplesInInput = buffer.getNumSamples();
    const int numChannels = buffer.getNumChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, numSamplesInInput);
        
    // Mono processing
    if (totalNumInputChannels == 1 && totalNumOutputChannels == 1)
    {
    }
    // Stereo processing
    // This is essentially the mono version but doubled up for both channels
    // We could generalise and refactor the processing to a separate function and call it for both channels as needed
    // instead of copying the code like this.
    else if (totalNumOutputChannels == 2)
    {

        float* leftData = buffer.getWritePointer(0);
        float* rightData = buffer.getWritePointer(1);
        for (int i = 0; i < numSamplesInInput; i++)
        {
            //chorus.process(leftData[i], rightData[i]);
            //delay.process(leftData[i], rightData[i]);
            //compressor.process(leftData[i], rightData[i]);
            //eq0.process(leftData[i], rightData[i]);
            //eq1.process(leftData[i], rightData[i]);
            bitCrusher.process(leftData[i], rightData[i]);

        }
    }
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
