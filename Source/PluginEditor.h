/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class VibratoAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    VibratoAudioProcessorEditor (VibratoAudioProcessor&);
    ~VibratoAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    VibratoAudioProcessor& audioProcessor;

    juce::Slider amountKnob;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> amountKnobAttach;
    juce::Label amountKnobLabel;

    juce::Slider speedKnob;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> speedKnobAttach;
    juce::Label speedKnobLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VibratoAudioProcessorEditor)
};
