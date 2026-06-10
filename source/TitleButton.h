/*
  ==============================================================================

    TitleButton.h
    Created: 28 Feb 2025 2:02:20pm
    Author:  Hugh Buntine

    Title button for DUMUMUB wavetable synthesizer.
    Toggles help screen overlay display when clicked.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DUMUMUButton.h"

//==============================================================================
/**
 * Title Button - Help Screen Toggle Control
 * 
 * Interactive title button that toggles the help screen overlay.
 * Provides access to user documentation and interface guidance.
 */
class DUMUMUB003AudioProcessorEditor;

class TitleButton  : public DUMUMUButton
{
public:
    TitleButton(DUMUMUB003AudioProcessor& p) : DUMUMUButton(p)
    {
        // Load title button state images from binary resources
        setUnpressed(ImageFileFormat::loadFrom(BinaryData::TITLE_BUTTON_UNPRESSED_png, BinaryData::TITLE_BUTTON_UNPRESSED_pngSize));
        setPressed(ImageFileFormat::loadFrom(BinaryData::TITLE_BUTTON_PRESSED_png, BinaryData::TITLE_BUTTON_PRESSED_pngSize));
        setUnpressedHover(ImageFileFormat::loadFrom(BinaryData::TITLE_BUTTON_UNPRESSED_HOVER_png, BinaryData::TITLE_BUTTON_UNPRESSED_HOVER_pngSize));
        setPressedHover(ImageFileFormat::loadFrom(BinaryData::TITLE_BUTTON_PRESSED_HOVER_png, BinaryData::TITLE_BUTTON_PRESSED_HOVER_pngSize));

        // Initialize button state
        setIsPressed(false);
        setIsHovered(false);
    }

    ~TitleButton() override
    {
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
    }

    // Toggle help screen display based on button state
    void action() override
  {
    // Access parent editor to toggle help overlay
        DUMUMUB003AudioProcessorEditor* parentComponent = dynamic_cast<DUMUMUB003AudioProcessorEditor*>(getParentComponent());
        if (parentComponent != nullptr)
        {
            parentComponent->toggleHelp(getIsPressed());
        }
  }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TitleButton)
};
