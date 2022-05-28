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
    // Setup global plugin settings
    juce::PropertiesFile::Options fileOptions;
    fileOptions.applicationName = "VibratoPlugin";
    fileOptions.filenameSuffix = "settings";
    fileOptions.folderName = "VibratoPlugin";
    fileOptions.osxLibrarySubFolder = "Application Support"; // Recommended by Apple
    fileOptions.commonToAllUsers = false;
    fileOptions.ignoreCaseOfKeyNames = false;
    fileOptions.storageFormat = juce::PropertiesFile::storeAsXML;
    properties.setStorageParameters(fileOptions);

    // Load global plugin scale
    auto* propertiesFile = properties.getUserSettings();
    double scale = 1.0;
    if (propertiesFile)
        scale = propertiesFile->getDoubleValue(WINDOW_SCALE_ID, 1.0);

    // Set plugin window dimensions and scale
    setSize (WINDOW_WIDTH * scale, WINDOW_HEIGHT * scale);
    juce::Rectangle<int> defaultSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    mainLayout.setBounds(defaultSize);

    // Allow for window to be resizable
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
    // Calculate, store and apply window scale
    const float scale = (float)getWidth() / (float)WINDOW_WIDTH;
    properties.getUserSettings()->setValue(WINDOW_SCALE_ID, scale);
    mainLayout.setTransform(juce::AffineTransform::scale(scale));
}
