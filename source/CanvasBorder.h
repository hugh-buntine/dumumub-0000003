/*
  ==============================================================================

    CanvasBorder.h
    Created: 28 Feb 2025 1:21:10pm
    Author:  Hugh Buntine

    Decorative border overlay component for the main canvas visualization.
    Provides visual framing without interfering with canvas interaction.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * Decorative border component that overlays the main canvas.
 * 
 * Renders a visual frame around the wavetable visualization area
 * while allowing all mouse interactions to pass through to the canvas below.
 */
class CanvasBorder  : public juce::Component
{
public:
    CanvasBorder()
    {
        // Load border graphics from binary resources
        canvasBorder = ImageFileFormat::loadFrom(BinaryData::CANVAS_BORDER_png, BinaryData::CANVAS_BORDER_pngSize);
    }

    ~CanvasBorder() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        // Render border frame scaled to component bounds
        g.drawImage(canvasBorder, 0, 0, getWidth(), getHeight(), 0, 0, canvasBorder.getWidth(), canvasBorder.getHeight());
    }

    void resized() override
    {
        // No child components to layout
    }

    bool hitTest(int x, int y) override {
        return false; // Allow all clicks to pass through to canvas below
    }


private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CanvasBorder)

    // Border Graphics
    Image canvasBorder;
};
