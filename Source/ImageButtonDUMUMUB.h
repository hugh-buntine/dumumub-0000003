/*
  ==============================================================================

    ImageButton.h
    Created: 28 Feb 2025 4:27:43pm
    Author:  Hugh Buntine

    Image waveform toggle button implementation.
    Controls image-based waveform generation in the synthesizer.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DUMUMUButton.h"

//==============================================================================
// Toggle button for enabling/disabling image-based waveform synthesis
class ImageButtonDUMUMUB  : public DUMUMUButton
{
public:
    ImageButtonDUMUMUB(DUMUMUB003AudioProcessor& p) : DUMUMUButton(p)
    {
        // Load image button visual assets
        setUnpressed(ImageFileFormat::loadFrom(BinaryData::IMAGE_BUTTON_UNPRESSED_png, BinaryData::IMAGE_BUTTON_UNPRESSED_pngSize));
        setPressed(ImageFileFormat::loadFrom(BinaryData::IMAGE_BUTTON_PRESSED_png, BinaryData::IMAGE_BUTTON_PRESSED_pngSize));
        setUnpressedHover(ImageFileFormat::loadFrom(BinaryData::IMAGE_BUTTON_UNPRESSED_HOVER_png, BinaryData::IMAGE_BUTTON_UNPRESSED_HOVER_pngSize));
        setPressedHover(ImageFileFormat::loadFrom(BinaryData::IMAGE_BUTTON_PRESSED_HOVER_png, BinaryData::IMAGE_BUTTON_PRESSED_HOVER_pngSize));

        // Initialize button state
        setIsPressed(false);
        setIsHovered(false);
    }

    ~ImageButtonDUMUMUB() override
    {
    }

    void resized() override
    {
        // No child components to layout
    }

    void action() override
    {
        // Toggle image-based waveform synthesis
        if (getIsPressed())
        {
            audioProcessor.setWaveformType("image", true);
        }
        else
        {
            audioProcessor.setWaveformType("image", false);
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImageButtonDUMUMUB)
};
