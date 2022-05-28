/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#define WINDOW_WIDTH 300
#define WINDOW_HEIGHT 250
#define WINDOW_SCALE_ID "WindowScale"

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "GUI/MainLayout.h"

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

    juce::ApplicationProperties properties;

    MainLayout mainLayout;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VibratoAudioProcessorEditor)
};
