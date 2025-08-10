/*
  ==============================================================================

    VolumeDisplay.h
    Created: 5 Mar 2025 3:16:58pm
    Author:  Hugh Buntine

    Volume display component for DUMUMUB wavetable synthesizer.
    Real-time stereo level meters with logarithmic scale and color coding.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * Volume Display - Real-Time Stereo Level Meters
 * 
 * Visual level monitoring component that displays left/right channel audio levels
 * using logarithmic scale with color-coded intensity (green/red) indicators.
 * Updates at 30Hz for smooth real-time response.
 */
class VolumeDisplay  : public juce::Component, private juce::Timer
{
public:
    VolumeDisplay(DUMUMUB003AudioProcessor& p);
    ~VolumeDisplay() override;

    // Render stereo level meters with color-coded indicators
    void paint (juce::Graphics&) override;

private:
    // Timer callback for 30Hz refresh rate
    void timerCallback() override;

    // Level meter graphics assets
    Image overlay;
    Image greenLight;
    Image redLight;

    // Reference to audio processor for level data
    DUMUMUB003AudioProcessor& audioProcessor;

    // Logarithmic scale intervals for level thresholds
    std::array<float, 16> intervals;

    // Spacing between level indicators
    int gap = 20;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VolumeDisplay)
};
