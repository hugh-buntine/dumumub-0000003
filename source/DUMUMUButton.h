/*
  ==============================================================================

    DUMUMUButton.h
    Created: 28 Feb 2025 1:39:34pm
    Author:  Hugh Buntine

    Abstract base class for custom toggle buttons with four-state visual feedback.
    Provides complete button implementation with hover states and toggle functionality.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
// Abstract base class for custom toggle buttons with interactive visual states
class DUMUMUButton  : public juce::Component
{
public:
    DUMUMUButton(DUMUMUB003AudioProcessor& p);
    ~DUMUMUButton() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // Mouse interaction handlers
    void mouseEnter(const MouseEvent& event) override;
    void mouseExit(const MouseEvent& event) override;
    void mouseDown(const MouseEvent& event) override;

    // Image setters for four button states
    void setUnpressed(Image image) { unpressed = image; }
    void setPressed(Image image) { pressed = image; }
    void setUnpressedHover(Image image) { unpressedHover = image; }
    void setPressedHover(Image image) { pressedHover = image; }

    // State management interface
    void setIsPressed(bool value) { isPressed = value; }
    void setIsHovered(bool value) { isHovered = value; }
    bool getIsPressed() { return isPressed; }
    bool getIsHovered() { return isHovered; }
    void toggleIsPressed() { isPressed = !isPressed; }

    DUMUMUB003AudioProcessor& getAudioProcessor() { return audioProcessor; }

    // Virtual method for button-specific behavior implementation
    virtual void action();

protected:
    DUMUMUB003AudioProcessor& audioProcessor;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DUMUMUButton)

    // Visual assets for different button states
    Image unpressed;
    Image pressed;
    Image unpressedHover;
    Image pressedHover;

    // Button state variables
    bool isPressed;
    bool isHovered;
};
