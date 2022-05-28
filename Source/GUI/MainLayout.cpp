/*
  ==============================================================================

    MainLayout.cpp
    Created: 28 May 2022 9:54:17am
    Author:  thesp

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainLayout.h"

//==============================================================================
MainLayout::MainLayout(VibratoAudioProcessor& p) : audioProcessor(p)
{
    juce::LookAndFeel::setDefaultLookAndFeel(&customLook);

    amountKnobAttach = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, AMOUNT_ID, amountKnob);

    amountKnob.setRange(0.f, 100.f);
    amountKnob.setTextValueSuffix("%");

    amountKnobLabel.setText("Amount", juce::dontSendNotification);
    amountKnobLabel.setJustificationType(juce::Justification::centred);
    amountKnobLabel.attachToComponent(&amountKnob, false);

    addAndMakeVisible(amountKnob);


    // ================================================================
    speedKnobAttach = std::make_unique <juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.treeState, SPEED_ID, speedKnob);

    speedKnob.setRange(SPEED_MIN, SPEED_MAX);
    speedKnob.setSkewFactorFromMidPoint(5.f);
    speedKnob.setTextValueSuffix(" Hz");

    speedKnobLabel.setText("Speed", juce::dontSendNotification);
    speedKnobLabel.setJustificationType(juce::Justification::centred);
    speedKnobLabel.attachToComponent(&speedKnob, false);

    addAndMakeVisible(speedKnob);
}

MainLayout::~MainLayout()
{
}

void MainLayout::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    auto rect = getLocalBounds();
    rect.removeFromTop(TOP_MARGIN);
    g.setColour(juce::Colours::lightslategrey);
    g.setFont(24.f);
    g.drawText("Vibrato", rect, juce::Justification::centredTop);
}

void MainLayout::resized()
{
    const int size = 100;

    auto rect = getLocalBounds();
    rect.removeFromTop(TOP_MARGIN);
    amountKnob.setBounds(rect.removeFromLeft(getWidth() / 2).withSizeKeepingCentre(size, size));

    speedKnob.setBounds(rect.withSizeKeepingCentre(size, size));
}
