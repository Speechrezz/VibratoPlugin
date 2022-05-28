/*
  ==============================================================================

    MainLayout.h
    Created: 28 May 2022 9:54:17am
    Author:  thesp

  ==============================================================================
*/

#pragma once

#define TOP_MARGIN 20

#include <JuceHeader.h>
#include "../PluginProcessor.h"
#include "customLook.h"
#include "Dial.h"

//==============================================================================
/*
*/
class MainLayout  : public juce::Component
{
public:
    MainLayout(VibratoAudioProcessor& p);
    ~MainLayout() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    VibratoAudioProcessor& audioProcessor;

    juce::CustomLook customLook;

    juce::Dial amountKnob;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> amountKnobAttach;
    juce::Label amountKnobLabel;

    juce::FrequencyDial speedKnob;
    std::unique_ptr <juce::AudioProcessorValueTreeState::SliderAttachment> speedKnobAttach;
    juce::Label speedKnobLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainLayout)
};
