/*
  ==============================================================================

    Canvas.h
    Created: 23 Feb 2025 3:43:39pm
    Author:  Hugh Buntine

    Interactive wavetable visualization and editing canvas.
    Displays real-time waveforms and allows mouse-based wavetable manipulation.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
 * Interactive canvas for wavetable visualization and real-time editing.
 * 
 * Features:
 * - Real-time wavetable visualization for both stereo channels
 * - Mouse-based wavetable editing with drag support
 * - Oscilloscope display for live audio monitoring
 * - Independent left/right channel toggle controls
 */
class Canvas  : public juce::Component, private juce::Timer
{
public:
    Canvas(DUMUMUB003AudioProcessor& p);
    ~Canvas() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    // Mouse Interaction
    void mouseDown(const MouseEvent& event) override;
    void mouseDrag(const MouseEvent& event) override;

    // Channel Control
    void toggleLeft(bool value) { left = value; }
    void toggleRight(bool value) { right = value; }

    // Visualization
    void drawOscilloscope(Graphics& g, std::array<float, 1024> array, Colour colour, int shift);

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Canvas)

    void timerCallback() override;

    // Mouse State
    int prevX;
    int prevY;

    // Channel State
    bool left;
    bool right;

    // Audio Processor Reference
    DUMUMUB003AudioProcessor& audioProcessor;

    // Background Graphics
    Image background;
};
