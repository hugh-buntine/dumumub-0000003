/*
  ==============================================================================

  LeftButton.h
  Created: 28 Feb 2025 3:34:59pm
  Author:  Hugh Buntine

  Left channel toggle button implementation.
  Controls left channel editing mode with persistent state.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DUMUMUButton.h"

// Forward declare DUMUMUB003AudioProcessorEditor
class DUMUMUB003AudioProcessorEditor;

//==============================================================================
// Toggle button for left channel editing mode
class LeftButton  : public DUMUMUButton
{
public:
  LeftButton(DUMUMUB003AudioProcessor& p) : DUMUMUButton(p)
  {
    // Load left button visual assets
    setUnpressed(ImageFileFormat::loadFrom(BinaryData::LEFT_BUTTON_UNPRESSED_png, BinaryData::LEFT_BUTTON_UNPRESSED_pngSize));
    setPressed(ImageFileFormat::loadFrom(BinaryData::LEFT_BUTTON_PRESSED_png, BinaryData::LEFT_BUTTON_PRESSED_pngSize));
    setUnpressedHover(ImageFileFormat::loadFrom(BinaryData::LEFT_BUTTON_UNPRESSED_HOVER_png, BinaryData::LEFT_BUTTON_UNPRESSED_HOVER_pngSize));
    setPressedHover(ImageFileFormat::loadFrom(BinaryData::LEFT_BUTTON_PRESSED_HOVER_png, BinaryData::LEFT_BUTTON_PRESSED_HOVER_pngSize));

    // Initialize with persistent state
    setIsPressed(p.getLeftChannelState()); // Restore saved state
    setIsHovered(false);
  }

  ~LeftButton() override
  {
  }

  void resized() override
  {
    // No child components to layout
  }

  void action() override
  {
    // Toggle left channel editing mode through editor
        DUMUMUB003AudioProcessorEditor* parentComponent = dynamic_cast<DUMUMUB003AudioProcessorEditor*>(getParentComponent());
        if (parentComponent != nullptr)
        {
            parentComponent->toggleLeftLight();
        }
  }

private:
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LeftButton)
};