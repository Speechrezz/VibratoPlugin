/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VibratoAudioProcessorEditor::VibratoAudioProcessorEditor (VibratoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p), mainLayout(p)
{
    setSize (WINDOW_WIDTH, WINDOW_HEIGHT);
    mainLayout.setBounds(getLocalBounds());

    setResizable(true, true);
    setResizeLimits(200, 200, 1200, 800);
    const float aspectRatio = (float)getWidth() / (float)getHeight();
    getConstrainer()->setFixedAspectRatio(aspectRatio);

    addAndMakeVisible(mainLayout);
}

VibratoAudioProcessorEditor::~VibratoAudioProcessorEditor()
{
}

//==============================================================================
void VibratoAudioProcessorEditor::paint (juce::Graphics& g)
{
}

void VibratoAudioProcessorEditor::resized()
{
    const float scale = (float)getWidth() / (float)WINDOW_WIDTH;
    mainLayout.setTransform(juce::AffineTransform::scale(scale));
}
