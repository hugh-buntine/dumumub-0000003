/*
  ==============================================================================

    SineButton.h
    Created: 28 Feb 2025 3:55:05pm
    Author:  Hugh Buntine

    Sine wave selection button for DUMUMUB wavetable synthesizer.
    Toggles sine waveform inclusion in wavetable operations.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DUMUMUButton.h"

//==============================================================================
/**
 * Sine Button - Sine Wave Selection Control
 * 
 * Toggle button for including sine waveforms in wavetable synthesis.
 * When active, sine waves are included in add/replace operations.
 */
class SineButton  : public DUMUMUButton
{
public:
    SineButton(DUMUMUB003AudioProcessor& p) : DUMUMUButton(p)
    {
        // Load sine button state images from binary resources
        setUnpressed(ImageFileFormat::loadFrom(BinaryData::SINE_WAVE_BUTTON_UNPRESSED_png, BinaryData::SINE_WAVE_BUTTON_UNPRESSED_pngSize));
        setPressed(ImageFileFormat::loadFrom(BinaryData::SINE_WAVE_BUTTON_PRESSED_png, BinaryData::SINE_WAVE_BUTTON_PRESSED_pngSize));
        setUnpressedHover(ImageFileFormat::loadFrom(BinaryData::SINE_WAVE_BUTTON_UNPRESSED_HOVER_png, BinaryData::SINE_WAVE_BUTTON_UNPRESSED_HOVER_pngSize));
        setPressedHover(ImageFileFormat::loadFrom(BinaryData::SINE_WAVE_BUTTON_PRESSED_HOVER_png, BinaryData::SINE_WAVE_BUTTON_PRESSED_HOVER_pngSize));

        // Initialize button state
        setIsPressed(false);
        setIsHovered(false);
    }

    ~SineButton() override
    {
    }

    void resized() override
    {
    }

    // Toggle sine waveform selection state
    void action() override
    {
        // Update processor waveform selection based on button state
        if (getIsPressed())
        {
            audioProcessor.setWaveformType("sine", true);
        }
        else
        {
            audioProcessor.setWaveformType("sine", false);
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SineButton)
};
