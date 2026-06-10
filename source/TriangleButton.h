/*
  ==============================================================================

    TriangleButton.h
    Created: 28 Feb 2025 4:08:10pm
    Author:  Hugh Buntine

    Triangle wave selection button for DUMUMUB wavetable synthesizer.
    Toggles triangle waveform inclusion in wavetable operations.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DUMUMUButton.h"

//==============================================================================
/**
 * Triangle Button - Triangle Wave Selection Control
 * 
 * Toggle button for including triangle waveforms in wavetable synthesis.
 * When active, triangle waves are included in add/replace operations.
 */
class TriangleButton  : public DUMUMUButton
{
public:
    TriangleButton(DUMUMUB003AudioProcessor& p) : DUMUMUButton(p)
    {
        // Load triangle button state images from binary resources
        setUnpressed(ImageFileFormat::loadFrom(BinaryData::TRIANGLE_WAVE_BUTTON_UNPRESSED_png, BinaryData::TRIANGLE_WAVE_BUTTON_UNPRESSED_pngSize));
        setPressed(ImageFileFormat::loadFrom(BinaryData::TRIANGLE_WAVE_BUTTON_PRESSED_png, BinaryData::TRIANGLE_WAVE_BUTTON_PRESSED_pngSize));
        setUnpressedHover(ImageFileFormat::loadFrom(BinaryData::TRIANGLE_WAVE_BUTTON_UNPRESSED_HOVER_png, BinaryData::TRIANGLE_WAVE_BUTTON_UNPRESSED_HOVER_pngSize));
        setPressedHover(ImageFileFormat::loadFrom(BinaryData::TRIANGLE_WAVE_BUTTON_PRESSED_HOVER_png, BinaryData::TRIANGLE_WAVE_BUTTON_PRESSED_HOVER_pngSize));

        // Initialize button state
        setIsPressed(false);
        setIsHovered(false);
    }

    ~TriangleButton() override
    {
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
    }

    // Toggle triangle waveform selection state
    void action() override
    {
        // Update processor waveform selection based on button state
        if (getIsPressed())
        {
            audioProcessor.setWaveformType("triangle", true);
        }
        else
        {
            audioProcessor.setWaveformType("triangle", false);
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TriangleButton)
};
