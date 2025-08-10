/*
  ==============================================================================

    VolumeDisplay.cpp
    Created: 5 Mar 2025 3:16:58pm
    Author:  Hugh Buntine

    Volume display implementation for DUMUMUB wavetable synthesizer.
    Real-time stereo level meters with color-coded intensity display.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "VolumeDisplay.h"

//==============================================================================
VolumeDisplay::VolumeDisplay(DUMUMUB003AudioProcessor& p) : audioProcessor(p)
{
    // Load level meter graphics from binary resources
    overlay = ImageFileFormat::loadFrom(BinaryData::EMITTER_CASE_png, BinaryData::EMITTER_CASE_pngSize);
    greenLight = ImageFileFormat::loadFrom(BinaryData::EMITTER_LIGHT_GREEN_png, BinaryData::EMITTER_LIGHT_GREEN_pngSize);
    redLight = ImageFileFormat::loadFrom(BinaryData::EMITTER_LIGHT_RED_png, BinaryData::EMITTER_LIGHT_RED_pngSize);
    
    // Start 30Hz refresh timer for smooth level display
    startTimerHz(30);

    // Define logarithmic scale intervals for accurate level representation
    intervals = { 0.001, 0.00158, 0.00251, 0.00398, 0.00631, 0.01, 0.0158, 0.0251, 0.0398, 0.0631, 0.1, 0.158, 0.251, 0.398 ,0.631, 1.0 };
}

VolumeDisplay::~VolumeDisplay()
{
}

// Render stereo level meters with color-coded intensity
void VolumeDisplay::paint (juce::Graphics& g)
{
  // Draw level indicators for each interval threshold
  for (int i = 0; i < intervals.size(); ++i)
  {
    // Left channel level display
    if (audioProcessor.getLoudnessL() > intervals[i])
    {
      if (i < 13)
      {
        g.drawImageAt(greenLight, 5 + (gap * i), 5); // Green for normal levels
      }
      else
      {
        g.drawImageAt(redLight, 5 + (gap * i), 5); // Red for high levels
      }
    }

    // Right channel level display
    if (audioProcessor.getLoudnessR() > intervals[i])
    {
      if (i < 13)
      {
        g.drawImageAt(greenLight, 5 + (gap * i), 17); // Green for normal levels
      }
      else
      {
        g.drawImageAt(redLight, 5 + (gap * i), 17); // Red for high levels
      }
    }
  }

    // Draw overlay frame on top of level indicators
    g.drawImageAt(overlay, 0, 0);
}

// Timer callback for smooth real-time level updates
void VolumeDisplay::timerCallback()
{
    repaint();
}
