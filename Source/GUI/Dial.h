/*
  ==============================================================================

    Dial.h
    Created: 11 Feb 2022 8:08:40pm
    Author:  thesp

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
namespace juce
{

class Dial : public Slider
{
public:
    // for dry & wet knobs
    Dial() 
    {
        setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
        setTextBoxStyle(Slider::TextBoxBelow, true, 150, 25);
        setTextBoxIsEditable(true);
        setMouseDragSensitivity(350);
    }

    void mouseDown(const MouseEvent& event)
    {
        Slider::mouseDown(event);

        setMouseCursor(MouseCursor::NoCursor);
    }

    // A bit buggy, needs more work
    void mouseDrag(const MouseEvent& event)
    {
        Slider::mouseDrag(event);

        if (event.mods.isShiftDown())
        {
            setVelocityBasedMode(true);
            setVelocityModeParameters(0.1, 1, 0.1, false);
        }
        else
            setVelocityBasedMode(false);

    }

    void mouseUp(const juce::MouseEvent& event)
    {
        Slider::mouseUp(event);

        auto mms = Desktop::getInstance().getMainMouseSource();
        mms.setScreenPosition(event.source.getLastMouseDownPosition());

        setMouseCursor(MouseCursor::NormalCursor);
    }

    // Makes mouse wheel less sensitive
    void mouseWheelMove(const MouseEvent& e, const MouseWheelDetails& wheel)
    {
        //DBG("Mouse wheel has moved");
        MouseWheelDetails wheelNew = wheel;

        wheelNew.deltaY /= 1.5f;

        Slider::mouseWheelMove(e, wheelNew);
    }
};

class GainDial : public Dial
{
public:
    // for dry & wet knobs
    GainDial() {

        setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        setTextBoxStyle(juce::Slider::TextBoxBelow, true, 150, 25);
        setTextBoxIsEditable(true);
        setTextValueSuffix(" dB");
    }
};

class FrequencyDial : public Dial
{
public:
    // for the LPF & HPF knobs
    FrequencyDial() {

        setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
        setTextBoxStyle(juce::Slider::TextBoxBelow, true, 150, 25);
        setTextBoxIsEditable(true);

    }

    String getTextFromValue(double value)
    {
        if (value >= 1000.0) {
            value = value / 100.0;
            return juce::String(value, 2) + " kHz";
        }

        return juce::String(value, 2) + " Hz";
    }
};

}