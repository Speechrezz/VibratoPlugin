/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#define AMOUNT_ID "amount"
#define AMOUNT_NAME "Amount"

#define SPEED_ID "speed"
#define SPEED_NAME "Speed"
#define SPEED_MIN 0.1f
#define SPEED_MAX 50.f

#include <JuceHeader.h>

//==============================================================================
/**
*/
class VibratoAudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    VibratoAudioProcessor();
    ~VibratoAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    juce::AudioProcessorValueTreeState treeState;

private:
    //==============================================================================
    juce::dsp::DelayLine<float, juce::dsp::DelayLineInterpolationTypes::Linear> delayLine{ 44100 };

    juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void processDelay(juce::dsp::ProcessContextReplacing<float> context);

    float prevPhase{ 0.f };
    juce::LinearSmoothedValue<float> magnitude{ 0 };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VibratoAudioProcessor)
};
