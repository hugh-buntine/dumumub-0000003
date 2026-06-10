/*
  ==============================================================================

    AudioButton.h
    Created: 28 Feb 2025 4:21:33pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DUMUMUButton.h"

//==============================================================================
/*
*/
class AudioButton  : public DUMUMUButton
{
public:
    AudioButton(DUMUMUB003AudioProcessor& p) : DUMUMUButton(p)
    {
        // SET THE IMAGES
        setUnpressed(ImageFileFormat::loadFrom(BinaryData::AUDIO_BUTTON_UNPRESSED_png, BinaryData::AUDIO_BUTTON_UNPRESSED_pngSize));
        setPressed(ImageFileFormat::loadFrom(BinaryData::AUDIO_BUTTON_PRESSED_png, BinaryData::AUDIO_BUTTON_PRESSED_pngSize));
        setUnpressedHover(ImageFileFormat::loadFrom(BinaryData::AUDIO_BUTTON_UNPRESSED_HOVER_png, BinaryData::AUDIO_BUTTON_UNPRESSED_HOVER_pngSize));
        setPressedHover(ImageFileFormat::loadFrom(BinaryData::AUDIO_BUTTON_PRESSED_HOVER_png, BinaryData::AUDIO_BUTTON_PRESSED_HOVER_pngSize));

        // SET THE STATE
        setIsPressed(false);
        setIsHovered(false);
    }

    ~AudioButton() override
    {
    }


    void resized() override
    {

    }

    void action() override
    {
        if (getIsPressed())
        {
            audioProcessor.setWaveformType("audio", true);
        }
        else
        {
            audioProcessor.setWaveformType("audio", false);
        }
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AudioButton)
};
