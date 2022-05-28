/*
  ==============================================================================

    CustomLook.h
    Created: 11 Feb 2022 3:28:57pm
    Author:  thesp

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace juce
{

class CustomLook : public LookAndFeel_V4
{
public:

    void drawRotarySlider(Graphics&, int x, int y, int width, int height, float sliderPosProportional, 
        float rotaryStartAngle, float rotaryEndAngle, Slider&) override;

    void drawLabel(Graphics& g, Label& label) override;

    void fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor) override;
};

}