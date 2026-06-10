/*
  ==============================================================================

    SawButton.h
    Created: 28 Feb 2025 3:59:48pm
    Author:  Hugh Buntine

    Sawtooth wave selection button for DUMUMUB wavetable synthesizer.
    Toggles sawtooth waveform inclusion in wavetable operations.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DUMUMUButton.h"

//==============================================================================
/**
 * Saw Button - Sawtooth Wave Selection Control
 * 
 * Toggle button for including sawtooth waveforms in wavetable synthesis.
 * When active, sawtooth waves are included in add/replace operations.
 */
class SawButton  : public DUMUMUButton
{
public:
    SawButton(DUMUMUB003AudioProcessor& p) : DUMUMUButton(p)
    {
        // Load sawtooth button state images from binary resources
        setUnpressed(ImageFileFormat::loadFrom(BinaryData::SAW_TOOTH_WAVE_BUTTON_UNPRESSED_png, BinaryData::SAW_TOOTH_WAVE_BUTTON_UNPRESSED_pngSize));
        setPressed(ImageFileFormat::loadFrom(BinaryData::SAW_TOOTH_WAVE_BUTTON_PRESSED_png, BinaryData::SAW_TOOTH_WAVE_BUTTON_PRESSED_pngSize));
        setUnpressedHover(ImageFileFormat::loadFrom(BinaryData::SAW_TOOTH_WAVE_BUTTON_UNPRESSED_HOVER_png, BinaryData::SAW_TOOTH_WAVE_BUTTON_UNPRESSED_HOVER_pngSize));
        setPressedHover(ImageFileFormat::loadFrom(BinaryData::SAW_TOOTH_WAVE_BUTTON_PRESSED_HOVER_png, BinaryData::SAW_TOOTH_WAVE_BUTTON_PRESSED_HOVER_pngSize));

        // Initialize button state
        setIsPressed(false);
        setIsHovered(false);
    }

    ~SawButton() override
    {
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
    }

    // Toggle sawtooth waveform selection state
    void action() override
    {
        // Update processor waveform selection based on button state
        if (getIsPressed())
        {
            audioProcessor.setWaveformType("saw", true);
        }
        else
        {
            audioProcessor.setWaveformType("saw", false);
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SawButton)
};
