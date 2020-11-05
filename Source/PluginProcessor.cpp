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
        ), parameters(*this, nullptr, juce::Identifier("TESTI"), createParameterLayout())
    #endif
{
    delayLenghtParam    = parameters.getRawParameterValue("delay_lenght");
    delayModAmountParam = parameters.getRawParameterValue("delay_modamount");
    delayLfoSpeedParam  = parameters.getRawParameterValue("delay_lfospeed");
    delayFeedbackParam  = parameters.getRawParameterValue("delay_feedback");
    delaywetDryMixParam = parameters.getRawParameterValue("delay_wetdry");
}

AudiopluginAudioProcessor::~AudiopluginAudioProcessor()
{
}

AudioProcessorValueTreeState::ParameterLayout AudiopluginAudioProcessor::createParameterLayout()
{
    // Here we can programatically add parameters to the parameter layout.
    AudioProcessorValueTreeState::ParameterLayout params;

    // Delay 
    // ID, name, min, max, default
    params.add(std::make_unique<AudioParameterFloat>("delay_lenght", "Delay Lenght (s)", 0.001f, 2.000f, 0.001f));
    params.add(std::make_unique<AudioParameterFloat>("delay_modamount", "Delay Modulation (ms)", 0.0f, 10.0f, 1.0f));
    params.add(std::make_unique<AudioParameterFloat>("delay_lfospeed", "Delay LFO Speed", 0.0f, 1.0f, 0.5f));
    params.add(std::make_unique<AudioParameterFloat>("delay_feedback", "Delay Feedback Amount", 0.0f, 1.0f, 0.1f));
    params.add(std::make_unique<AudioParameterFloat>("delay_wetdry", "Delay Wet Dry Mix", 0.0f, 1.0f, 0.5f));

    return params;
}

//==============================================================================
const String AudiopluginAudioProcessor::getName() const
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

    const int maxDelayLineInSamples = 10 * sampleRate;

    // To assign a new object to std::unique_ptr, we call its reset().
    // This deletes the old object, if one exists, and moves to point to the new object.
    leftDelayLine.reset(new DelayLine(maxDelayLineInSamples));
    rightDelayLine.reset(new DelayLine(maxDelayLineInSamples));

    // Create our LFOs
    const double frequencyInHz = *delayLfoSpeedParam;
    const double leftStartingPhase = MathConstants<double>::twoPi * 0;
    const double rightStartingPhase = MathConstants<double>::twoPi * 0.25;

    leftLfoOsc.reset(new SineOscillator(sampleRate, frequencyInHz, leftStartingPhase));
    rightLfoOsc.reset(new SineOscillator(sampleRate, frequencyInHz, rightStartingPhase));

    // These will be used for feedback, initialise to zero
    prevLeftDelayedSample = 0;
    prevRightDelayedSample = 0;
}

void AudiopluginAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool AudiopluginAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void AudiopluginAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    const float maxAmplitudeInSeconds = *delayModAmountParam / 1000;
    const float baseDelayInSeconds = *delayLenghtParam;
    const float wetDryRatio = *delaywetDryMixParam;
    const double samplerate = getSampleRate();

    const float feedbackGain = *delayFeedbackParam;

    const int numSamplesInInput = buffer.getNumSamples();

    // Update the frequency of the LFO
    const float lfoFreqInSecs = *delayLfoSpeedParam;
    leftLfoOsc->setFrequency(lfoFreqInSecs);

    // Mono processing
    if (totalNumInputChannels == 1 && totalNumOutputChannels == 1)
    {
        // Get access to the audio channel
        float* monoData = buffer.getWritePointer(0);

        // Process each sample
        for (int i = 0; i < numSamplesInInput; i++)
        {
            float inputSample = monoData[i];

            // Since we have an LFO, the delay in samples changes for every sample
            float delayInSamples = baseDelayInSeconds * samplerate + leftLfoOsc->getNextSample() * maxAmplitudeInSeconds * samplerate;

            // Push the sample to the delay line, and add the previous sample for the feedback effect
            leftDelayLine->pushSample(inputSample + prevLeftDelayedSample * feedbackGain);

            // Get the new delayed sample
            float wetSample = leftDelayLine->getDelayedSampleInterp(delayInSamples);
            float drySample = monoData[i];

            // Replace the output sample with a mixture of wet and dry samples
            monoData[i] = wetDryRatio * wetSample + (1 - wetDryRatio) * drySample;

            // Store what the previous sample was to use it the next time around
            prevLeftDelayedSample = wetSample;
        }
    }
    // Stereo processing
    // This is essentially the mono version but doubled up for both channels
    // We could generalise and refactor the processing to a separate function and call it for both channels as needed
    // instead of copying the code like this.
    else if (totalNumOutputChannels == 2)
    {
        // update the frequency of lfo
        rightLfoOsc->setFrequency(lfoFreqInSecs);

        float* leftData = buffer.getWritePointer(0);
        float* rightData = buffer.getWritePointer(1);

        for (int i = 0; i < numSamplesInInput; i++)
        {
            float leftSample = leftData[i];
            float rightSample;

            // Handle stereo to stereo
            if (totalNumInputChannels == 2)
            {
                rightSample = rightData[i];
            }
            // For mono to stereo, use the left channel input for right channel as well
            else
            {
                rightSample = leftSample;
            }

            float leftDelayInSamples = baseDelayInSeconds * samplerate + leftLfoOsc->getNextSample() * maxAmplitudeInSeconds * samplerate;
            float rightDelayInSamples = baseDelayInSeconds * samplerate + rightLfoOsc->getNextSample() * maxAmplitudeInSeconds * samplerate;

            leftDelayLine->pushSample(leftSample + prevLeftDelayedSample * feedbackGain);
            rightDelayLine->pushSample(rightSample + prevRightDelayedSample * feedbackGain);

            float leftWetSample = leftDelayLine->getDelayedSampleInterp(leftDelayInSamples);
            float rightWetSample = rightDelayLine->getDelayedSampleInterp(rightDelayInSamples);

            float leftDrySample = leftData[i];
            float rightDrySample = rightData[i];

            leftData[i] = wetDryRatio * leftWetSample + (1 - wetDryRatio) * leftDrySample;
            rightData[i] = wetDryRatio * rightWetSample + (1 - wetDryRatio) * rightDrySample;

            prevLeftDelayedSample = leftWetSample;
            prevRightDelayedSample = rightWetSample;
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

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AudiopluginAudioProcessor();
}
