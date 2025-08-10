/*
  ==============================================================================

    SquareButton.h
    Created: 28 Feb 2025 4:04:37pm
    Author:  Hugh Buntine

    Square wave selection button for DUMUMUB wavetable synthesizer.
    Toggles square waveform inclusion in wavetable operations.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DUMUMUButton.h"

//==============================================================================
/**
 * Square Button - Square Wave Selection Control
 * 
 * Toggle button for including square waveforms in wavetable synthesis.
 * When active, square waves are included in add/replace operations.
 */
class SquareButton  : public DUMUMUButton
{
public:
    SquareButton(DUMUMUB003AudioProcessor& p) : DUMUMUButton(p)
    {
        // Load square button state images from binary resources
        setUnpressed(ImageFileFormat::loadFrom(BinaryData::SQUARE_WAVE_BUTTON_UNPRESSED_png, BinaryData::SQUARE_WAVE_BUTTON_UNPRESSED_pngSize));
        setPressed(ImageFileFormat::loadFrom(BinaryData::SQUARE_WAVE_BUTTON_PRESSED_png, BinaryData::SQUARE_WAVE_BUTTON_PRESSED_pngSize));
        setUnpressedHover(ImageFileFormat::loadFrom(BinaryData::SQUARE_WAVE_BUTTON_UNPRESSED_HOVER_png, BinaryData::SQUARE_WAVE_BUTTON_UNPRESSED_HOVER_pngSize));
        setPressedHover(ImageFileFormat::loadFrom(BinaryData::SQUARE_WAVE_BUTTON_PRESSED_HOVER_png, BinaryData::SQUARE_WAVE_BUTTON_PRESSED_HOVER_pngSize));

        // Initialize button state
        setIsPressed(false);
        setIsHovered(false);
    }

    ~SquareButton() override
    {
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
    }

    // Toggle square waveform selection state
    void action() override
    {
        // Update processor waveform selection based on button state
        if (getIsPressed())
        {
            audioProcessor.setWaveformType("square", true);
        }
        else
        {
            audioProcessor.setWaveformType("square", false);
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SquareButton)
};
