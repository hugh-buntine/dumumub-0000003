/*
  ==============================================================================

    ReplaceButton.h
    Created: 28 Feb 2025 3:51:27pm
    Author:  Hugh Buntine

    Replace button implementation for DUMUMUB wavetable synthesizer.
    Provides wavetable replacement functionality with channel-aware operation.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DUMUMUButton.h"

class DUMUMUB003AudioProcessorEditor;

//==============================================================================
/**
 * Replace Button - Wavetable Replacement Control
 * 
 * Specialized button that replaces existing wavetable content with selected waveforms.
 * Unlike the Add button which blends waveforms, this completely overwrites the current
 * wavetable data. Operates on left/right channels based on stereo light state.
 */
class ReplaceButton  : public DUMUMUButton
{
public:
    ReplaceButton(DUMUMUB003AudioProcessor& p) : DUMUMUButton(p)
    {
        // Load button state images from binary resources
        setUnpressed(ImageFileFormat::loadFrom(BinaryData::REPLACE_BUTTON_UNPRESSED_png, BinaryData::REPLACE_BUTTON_UNPRESSED_pngSize));
        setPressed(ImageFileFormat::loadFrom(BinaryData::REPLACE_BUTTON_PRESSED_png, BinaryData::REPLACE_BUTTON_PRESSED_pngSize));
        setUnpressedHover(ImageFileFormat::loadFrom(BinaryData::REPLACE_BUTTON_UNPRESSED_HOVER_png, BinaryData::REPLACE_BUTTON_UNPRESSED_HOVER_pngSize));
        setPressedHover(ImageFileFormat::loadFrom(BinaryData::REPLACE_BUTTON_PRESSED_HOVER_png, BinaryData::REPLACE_BUTTON_PRESSED_HOVER_pngSize));

        // Initialize button state
        setIsPressed(false);
        setIsHovered(false);
    }

    ~ReplaceButton() override
    {
    }


    void resized() override
    {
    }

    // Core button action - executes wavetable replacement operation
    void action() override
  {
    // Access parent editor for channel state information
      DUMUMUB003AudioProcessorEditor* parentComponent = dynamic_cast<DUMUMUB003AudioProcessorEditor*>(getParentComponent());
        if (parentComponent != nullptr)
        {
            bool leftOn = parentComponent->getLeftOn();
            bool rightOn = parentComponent->getRightOn();

            // Execute wavetable replacement based on active channels
            if (leftOn)
            {
                audioProcessor.replaceTableToL();
            }
            if (rightOn)
            {
                audioProcessor.replaceTableToR();
            }
            
            // Update visual display immediately
            parentComponent->repaintCanvas();
        }
  }

  // Reset button visual state on mouse release
  void mouseUp(const MouseEvent& event) override
  {
    setIsPressed(false);
    repaint();
  }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ReplaceButton)
};
