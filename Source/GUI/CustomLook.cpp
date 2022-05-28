/*
  ==============================================================================

    CustomLook.cpp
    Created: 11 Feb 2022 3:28:57pm
    Author:  thesp

  ==============================================================================
*/

#include "CustomLook.h"

namespace juce {

// Custom rotary slider
void CustomLook::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
    float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    // If mouse is hovering over
    const bool highlight = slider.isMouseOverOrDragging();

	const float thickness = highlight ? 5.f : 4.f;

	// Radius of knob
	float radius = juce::jmin(width / 2, height / 2) - 5;
	// Centre point (centreX, centreY) of knob
	float centreX = x + width * 0.5f;
	float centreY = y + radius + 12.f;

	// current angle of the slider
	float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

	// Draw path of the slider backgound (in darker background colour)
	juce::Path backgroundArc;
	backgroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, rotaryEndAngle, true);
	juce::Colour myColour = findColour(0x1005700);
	myColour = myColour.brighter(0.2f);
	g.setColour(myColour);
	g.strokePath(backgroundArc, juce::PathStrokeType(thickness, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

	// Draw path of slider foreground (in white)
	juce::Path foregroundArc;
	foregroundArc.addCentredArc(centreX, centreY, radius, radius, 0.0f, rotaryStartAngle, angle, true);
	g.setColour(juce::Colours::white);
	g.strokePath(foregroundArc, juce::PathStrokeType(thickness, juce::PathStrokeType::curved, juce::PathStrokeType::rounded));

}

// Slider textbox
void CustomLook::drawLabel(juce::Graphics& g, juce::Label& label)
{
	g.setColour(juce::Colours::lightslategrey);
    
	juce::String text = label.getText();
	//text.append(text.getFloatValue(), );
	int width = label.getWidth();
	int height = label.getHeight();
	g.setFont(juce::Font(height - 8.f, juce::Font::plain));
	g.drawText(text, 0, 0, width, height, juce::Justification::centred);
}

void CustomLook::fillTextEditorBackground(Graphics& g, int width, int height, TextEditor& textEditor)
{
    g.setColour(Colours::darkgrey.darker(0.6f));
    Rectangle<float> rect(width, height);
    rect = rect.withSizeKeepingCentre(rect.getWidth() * 0.8f, rect.getHeight() * 0.8f);
    g.fillRoundedRectangle(rect.reduced(1), 10);
    textEditor.setJustification(Justification::centred);
    //textEditor.setFont(juce::Font(height - 8.f, juce::Font::plain));
}

}