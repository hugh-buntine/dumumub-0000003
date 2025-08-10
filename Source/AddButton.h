/*
  ==============================================================================

    AddButton.h
    Created: 28 Feb 2025 3:46:24pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DUMUMUButton.h"

class DUMUMUB003AudioProcessorEditor;

//==============================================================================
/*
*/
class AddButton  : public DUMUMUButton
{
public:
    AddButton(DUMUMUB003AudioProcessor& p) : DUMUMUButton(p)
    {
        // SET THE IMAGES
        setUnpressed(ImageFileFormat::loadFrom(BinaryData::ADD_BUTTON_UNPRESSED_png, BinaryData::ADD_BUTTON_UNPRESSED_pngSize));
        setPressed(ImageFileFormat::loadFrom(BinaryData::ADD_BUTTON_PRESSED_png, BinaryData::ADD_BUTTON_PRESSED_pngSize));
        setUnpressedHover(ImageFileFormat::loadFrom(BinaryData::ADD_BUTTON_UNPRESSED_HOVER_png, BinaryData::ADD_BUTTON_UNPRESSED_HOVER_pngSize));
        setPressedHover(ImageFileFormat::loadFrom(BinaryData::ADD_BUTTON_PRESSED_HOVER_png, BinaryData::ADD_BUTTON_PRESSED_HOVER_pngSize));

        // SET THE STATE
        setIsPressed(false);
        setIsHovered(false);
    }

    ~AddButton() override
    {
    }


    void resized() override
    {

    }

      void action() override
  {
    // Access the editor
      DUMUMUB003AudioProcessorEditor* parentComponent = dynamic_cast<DUMUMUB003AudioProcessorEditor*>(getParentComponent());
        if (parentComponent != nullptr) // if we found it
        {
            bool leftOn = parentComponent->getLeftOn();
            bool rightOn = parentComponent->getRightOn();

            if (leftOn)
            {
                audioProcessor.addWaveTableToL();
            }
            if (rightOn)
            {
                audioProcessor.addWaveTableToR();
            }
            parentComponent->repaintCanvas();
        }
  }

  void mouseUp(const MouseEvent& event) override
  {
    setIsPressed(false);
    repaint();
  }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AddButton)
};
