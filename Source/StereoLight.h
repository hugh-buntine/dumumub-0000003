/*
  ==============================================================================

    StereoLight.h
    Created: 28 Feb 2025 4:52:25pm
    Author:  Hugh Buntine

    Stereo channel indicator light for DUMUMUB wavetable synthesizer.
    Visual feedback component for left/right channel selection states.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * Stereo Light - Channel State Indicator
 * 
 * Visual indicator component that displays on/off state for stereo channels.
 * Used to show which channels are active for wavetable operations.
 * Displays different images based on current state.
 */
class StereoLight  : public juce::Component
{
public:
    StereoLight()
    {
        // Initialize component state and load indicator images
        isOn = true;

        on = ImageFileFormat::loadFrom(BinaryData::ON_png, BinaryData::ON_pngSize);
        off = ImageFileFormat::loadFrom(BinaryData::OFF_png, BinaryData::OFF_pngSize);
    }

    ~StereoLight() override
    {
    }

    // Render appropriate state image
    void paint (juce::Graphics& g) override
    {
        if (isOn)
        {
            g.drawImageAt(on, 0, 0);
        }
        else
        {
            g.drawImageAt(off, 0, 0);
        }
    }

    void resized() override
    {
        // This method is where you should set the bounds of any child
        // components that your component contains..
    }

    // Set light state and trigger visual update
    void setOn(bool on)
    {
        isOn = on;
        repaint();
    }

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StereoLight)

    // State indicator images
    Image on;
    Image off;

    // Current light state
    bool isOn;
};
