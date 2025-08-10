/*
  ==============================================================================

    RightButton.h
    Created: 28 Feb 2025 3:40:58pm
    Author:  Hugh Buntine

    Right channel toggle button for DUMUMUB wavetable synthesizer.
    Controls right channel selection state for stereo operations.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DUMUMUButton.h"

//==============================================================================
/**
 * Right Button - Right Channel Toggle Control
 * 
 * Toggles right channel activation for stereo wavetable operations.
 * Works in conjunction with left button to enable independent channel control.
 * State is persisted and restored between sessions.
 */

class DUMUMUB003AudioProcessorEditor;

class RightButton  : public DUMUMUButton
{
public:
    RightButton(DUMUMUB003AudioProcessor& p) : DUMUMUButton(p)
    {
        // Load button state images from binary resources
        setUnpressed(ImageFileFormat::loadFrom(BinaryData::RIGHT_BUTTON_UNPRESSED_png, BinaryData::RIGHT_BUTTON_UNPRESSED_pngSize));
        setPressed(ImageFileFormat::loadFrom(BinaryData::RIGHT_BUTTON_PRESSED_png, BinaryData::RIGHT_BUTTON_PRESSED_pngSize));
        setUnpressedHover(ImageFileFormat::loadFrom(BinaryData::RIGHT_BUTTON_UNPRESSED_HOVER_png, BinaryData::RIGHT_BUTTON_UNPRESSED_HOVER_pngSize));
        setPressedHover(ImageFileFormat::loadFrom(BinaryData::RIGHT_BUTTON_PRESSED_HOVER_png, BinaryData::RIGHT_BUTTON_PRESSED_HOVER_pngSize));

        // Initialize button state from saved processor state
        setIsPressed(p.getRightChannelState()); // Restore saved state
        setIsHovered(false);
    }

    ~RightButton() override
    {
    }


    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
    }

    // Toggle right channel state via parent editor
    void action() override
  {
    // Access parent editor to toggle right channel state
        DUMUMUB003AudioProcessorEditor* parentComponent = dynamic_cast<DUMUMUB003AudioProcessorEditor*>(getParentComponent());
        if (parentComponent != nullptr)
        {
            parentComponent->toggleRightLight();
        }
  }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (RightButton)
};
