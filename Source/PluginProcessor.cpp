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
    , parameters(*this, nullptr, juce::Identifier("params"), createParameterLayout())
    , eq0(*this, "eq0", 0, samplerate) // call the constructors of FilterBands on initialisation
    , eq1(*this, "eq1", 1, samplerate)
    , compressor0(*this, "comp0", samplerate)
    , compressor1(*this, "comp1", samplerate)
    , bitCrusher(*this, samplerate)
    , chorus(*this, samplerate)
    , delay(*this, samplerate)
    , distortion(*this, samplerate)
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

    eq0.prepare(numChannels);
    eq1.prepare(numChannels);
    compressor0.prepare(samplesPerBlock);
    compressor1.prepare(samplesPerBlock);
    bitCrusher.prepare();

    chorus.prepareToPlay(sampleRate);
    delay.prepareToPlay(sampleRate);
    distortion.prepareToPlay(sampleRate, samplesPerBlock, getTotalNumInputChannels());
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
            eq0.process(leftData[i], rightData[i]);
            eq1.process(leftData[i], rightData[i]);
            compressor0.process(leftData[i], rightData[i]);
            compressor1.process(leftData[i], rightData[i]);
            bitCrusher.process(leftData[i], rightData[i]);
            chorus.process(leftData[i], rightData[i]);
            delay.process(leftData[i], rightData[i]);
        }
        distortion.process(buffer);
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
