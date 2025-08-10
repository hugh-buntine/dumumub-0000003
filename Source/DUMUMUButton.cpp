/*
  ==============================================================================

    DUMUMUButton.cpp
    Created: 28 Feb 2025 1:39:34pm
    Author:  Hugh Buntine

    Base class implementation for custom toggle buttons with hover states.
    Provides four-state visual feedback and toggle functionality.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DUMUMUButton.h"

//==============================================================================
DUMUMUButton::DUMUMUButton(DUMUMUB003AudioProcessor& p) : audioProcessor(p)
{
    // Initialize button state
    isPressed = false;
    isHovered = false;
}

DUMUMUButton::~DUMUMUButton()
{
}

void DUMUMUButton::paint (juce::Graphics& g)
{
    // Render appropriate button state based on pressed/hovered combination
    if (isPressed && isHovered) {
        g.drawImage(pressedHover, 0, 0, getWidth(), getHeight(), 0, 0, pressedHover.getWidth(), pressedHover.getHeight());
    } else if (isPressed) {
        g.drawImage(pressed, 0, 0, getWidth(), getHeight(), 0, 0, pressed.getWidth(), pressed.getHeight());
    } else if (isHovered) {
        g.drawImage(unpressedHover, 0, 0, getWidth(), getHeight(), 0, 0, unpressedHover.getWidth(), unpressedHover.getHeight());
    } else {
        g.drawImage(unpressed, 0, 0, getWidth(), getHeight(), 0, 0, unpressed.getWidth(), unpressed.getHeight());
    }
}

void DUMUMUButton::mouseEnter(const MouseEvent& event) {
  isHovered = true;
  repaint(); // Trigger visual state update
}

void DUMUMUButton::mouseExit(const MouseEvent& event) {
  isHovered = false;
  repaint(); // Trigger visual state update
}

void DUMUMUButton::mouseDown(const MouseEvent& event) {
  isPressed = !isPressed; // Toggle button state
  action(); // Execute button-specific behavior
  repaint(); // Update visual appearance
}


void DUMUMUButton::resized()
{
    // No child components to layout in base class
}

void DUMUMUButton::action()
{
    // Override in derived classes to implement button-specific behavior
}
