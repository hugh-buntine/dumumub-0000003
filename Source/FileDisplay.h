/*
  ==============================================================================

    FileDisplay.h
    Created: 28 Feb 2025 4:37:43pm
    Author:  Hugh Buntine

    Component for displaying loaded audio/image filenames with custom typography.
    Shows different states: waiting for file drop vs displaying loaded filename.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * Display component for showing loaded file names with custom styling.
 * Switches between "waiting" and "loaded" visual states.
 */
class FileDisplay  : public juce::Component
{
public:
    FileDisplay()
    {
        // Load UI state images
        empty = ImageFileFormat::loadFrom(BinaryData::AUDIO_TEXT_EMPTY_png, BinaryData::AUDIO_TEXT_EMPTY_pngSize);
        dropWaiting = ImageFileFormat::loadFrom(BinaryData::AUDIO_TEXT_DROP_png, BinaryData::AUDIO_TEXT_DROP_pngSize);

        waiting = true;

    }

    ~FileDisplay() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        if (waiting)
        {
            g.drawImage(dropWaiting, 0, 0, getWidth(), getHeight(), 0, 0, dropWaiting.getWidth(), dropWaiting.getHeight());
        }
        else
        {
            g.drawImage(empty, 0, 0, getWidth(), getHeight(), 0, 0, empty.getWidth(), empty.getHeight());

            // Render filename with spaced typography
            g.setColour(Colour::fromRGBA(20, 20, 0, 255));
            Font font(Font::getDefaultSansSerifFontName(), 13.0f, Font::plain);
            font.setExtraKerningFactor(0.3f); // More spacing between letters
            g.setFont(font);
            g.drawText(fileName, 5, 0, getWidth() - 10, getHeight(), Justification::centred);
        }
    }

    void resized() override
    {
        // No child components to layout
    }

    void setFileName(String name)
    {
        fileName = name;
        waiting = false;
        repaint();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileDisplay)

    Image empty;
    Image dropWaiting;

    String fileName;

    bool waiting;
};
