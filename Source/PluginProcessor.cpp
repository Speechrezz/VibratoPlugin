/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define TWOPI 6.28318
#define PI 3.14159

//==============================================================================
VibratoAudioProcessor::VibratoAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                    treeState(*this, nullptr, "PARAMETER", { createParameterLayout() })
#endif
{
}

VibratoAudioProcessor::~VibratoAudioProcessor()
{
}

juce::AudioProcessorValueTreeState::ParameterLayout VibratoAudioProcessor::createParameterLayout() {
    // Add parameters to the paramater tree. Allows for saving/loading and automating of paramaters

    std::vector < std::unique_ptr <juce::RangedAudioParameter> > params;

    auto speedParam = std::make_unique<juce::AudioParameterFloat>(SPEED_ID, SPEED_NAME, SPEED_MIN, SPEED_MAX, 10.f);
    params.push_back(std::move(speedParam));

    auto amountParam = std::make_unique<juce::AudioParameterFloat>(AMOUNT_ID, AMOUNT_NAME, 0.f, 100.f, 20.f);
    params.push_back(std::move(amountParam));

    return { params.begin(), params.end() };
}

//==============================================================================
const juce::String VibratoAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VibratoAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VibratoAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VibratoAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VibratoAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VibratoAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VibratoAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VibratoAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VibratoAudioProcessor::getProgramName (int index)
{
    return {};
}

void VibratoAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VibratoAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    juce::dsp::ProcessSpec spec;
    spec.sampleRate = sampleRate;
    spec.maximumBlockSize = samplesPerBlock;
    spec.numChannels = getTotalNumOutputChannels();

    delayLine.reset();
    delayLine.prepare(spec);

    magnitude.reset(sampleRate, 0.01);
}

void VibratoAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VibratoAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void VibratoAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    auto audioBlock = juce::dsp::AudioBlock<float>(buffer).getSubsetChannelBlock(0, (size_t)totalNumInputChannels);
    auto context = juce::dsp::ProcessContextReplacing<float>(audioBlock);

    processDelay(context);

}

void VibratoAudioProcessor::processDelay(juce::dsp::ProcessContextReplacing<float> context)
{
    /*
    * In order to allow for realtime changing of the 'speed', we must calculate the current
    * magnitude in terms of the phase.
    * 
    * For the system to oscillate x times a second (for a sine function):
    * 
    *   0->2pi in 1/x a second
    *   0->2pi in 1/x * samplerate
    * 
    * So each sample, the phase must increase by (phase delta):
    * 
    *               2pi
    *   pd = ------------------
    *         1/x * samplerate
    * 
    * To make this more flexible for other waveforms (other than sine), we can remove the 2pi:
    * 
    *                1                 x
    *   pd = ------------------ = ------------
    *         1/x * samplerate     samplerate
    * 
    * And we can multiply the 2pi later, when specifically doing the sine waveform:
    * 
    *   delay = 1 + magnitude * (1 + sin(2pi * phase))
    *   phase += pd
    * 
    * The extra math is to ensure the delay is never below +1. Weird stuff happens when the
    * the delay is below 1.
    * 
    * This way, the phase will NOT drastically change when changing the speed, instead the
    * phase delta (pd) will change.
    */

    const auto bufferSize = context.getInputBlock().getNumSamples();
    const auto numChannels = context.getInputBlock().getNumChannels();

    const auto bufferOut = context.getOutputBlock();

    const auto amount = treeState.getRawParameterValue(AMOUNT_ID)->load() * 2.f;
    const auto prevMag = magnitude.getCurrentValue();

    const auto speed = treeState.getRawParameterValue(SPEED_ID)->load() / 2.f;

    const float pd = speed / getSampleRate(); // phase delta
    float phase;

    // Iterate through each sample
    for (int channel = 0; channel < numChannels; ++channel)
    {
        // Reset phase, so both channels will behave identically
        phase = prevPhase;

        // Reset magnitude, so both channels will behave identically
        magnitude.setCurrentAndTargetValue(prevMag);
        magnitude.setTargetValue(amount);

        auto* samplesIn = context.getInputBlock().getChannelPointer(channel);

        // Process audio...
        for (int sample = 0; sample < bufferSize; ++sample) {

            // Update delay times (using sine function)
            const float newDelay = 1.f + magnitude.getNextValue() * (1.f + std::sin(TWOPI * phase));
            phase += pd;
            delayLine.setDelay(newDelay);

            float delayedSample = delayLine.popSample(channel);

            float delayIn = samplesIn[sample];
            delayLine.pushSample(channel, delayIn);

            // Stores delay signal in output
            bufferOut.setSample(channel, sample, delayedSample);
        }
    }

    // Keep the phase between 0 and 2pi
    prevPhase = std::fmod(phase, 1.f);
    // DBG("pd: " << pd * 360.f << ", phase: " << prevPhase * 360.f);
}

//==============================================================================
bool VibratoAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VibratoAudioProcessor::createEditor()
{
    return new VibratoAudioProcessorEditor (*this);
}

//==============================================================================
void VibratoAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = treeState.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void VibratoAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    // 'If' statements are for error checking
    if (xmlState != nullptr) {
        if (xmlState->hasTagName(treeState.state.getType())) {
            treeState.replaceState(juce::ValueTree::fromXml(*xmlState));
        }
    }
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VibratoAudioProcessor();
}
