/*
  ==============================================================================

    HelpScreen.h
    Created: 3 Mar 2025 2:56:02pm
    Author:  Hugh Buntine

    Overlay help system displaying usage instructions and controls.
    Can be toggled on/off and allows click-through when inactive.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * Overlay help screen component providing user guidance.
 * Displays instructions for wavetable editing, file import, and controls.
 */
class HelpScreen  : public juce::Component
{
public:
    HelpScreen();
    ~HelpScreen() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    bool hitTest(int x, int y) override {
        return false; // Allows clicks to pass through
    }

    void toggle(bool value) { isOn = value; repaint(); }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HelpScreen)

    Image picture;

    bool isOn;
};
