/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VibratoAudioProcessorEditor::VibratoAudioProcessorEditor (VibratoAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    setSize (400, 300);

    amountKnobAttach = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, AMOUNT_ID, amountKnob);

    amountKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    amountKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 150, 25);
    amountKnob.setTextBoxIsEditable(true);
    amountKnob.setRange(0.f, 100.f);
    amountKnob.setTextValueSuffix("%");

    amountKnobLabel.setText("Amount", juce::dontSendNotification);
    amountKnobLabel.setJustificationType(juce::Justification::centred);
    amountKnobLabel.attachToComponent(&amountKnob, false);

    addAndMakeVisible(amountKnob);


    // ================================================================
    speedKnobAttach = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, SPEED_ID, speedKnob);

    speedKnob.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    speedKnob.setTextBoxStyle(juce::Slider::TextBoxBelow, true, 150, 25);
    speedKnob.setTextBoxIsEditable(true);
    speedKnob.setRange(SPEED_MIN, SPEED_MAX);
    speedKnob.setSkewFactorFromMidPoint(2.f);
    speedKnob.setTextValueSuffix(" Hz");

    speedKnobLabel.setText("Speed", juce::dontSendNotification);
    speedKnobLabel.setJustificationType(juce::Justification::centred);
    speedKnobLabel.attachToComponent(&speedKnob, false);

    addAndMakeVisible(speedKnob);
}

VibratoAudioProcessorEditor::~VibratoAudioProcessorEditor()
{
}

//==============================================================================
void VibratoAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void VibratoAudioProcessorEditor::resized()
{
    amountKnob.setBounds(100, 100, 100, 100);

    speedKnob.setBounds(200, 100, 100, 100);
}
