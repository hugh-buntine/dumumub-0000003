/*
  ==============================================================================

    KnobBackground.cpp
    Created: 7 Aug 2025 11:49:49am
    Author:  Hugh Buntine

    Background image component for knob controls.
    Provides visual backdrop with mouse-click pass-through functionality.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "KnobBackground.h"

//==============================================================================
KnobBackground::KnobBackground()
{
    // Load background image and enable click pass-through
    knobBackgroundImage = ImageFileFormat::loadFrom(BinaryData::KNOBS_BACKGROUND_png, BinaryData::KNOBS_BACKGROUND_pngSize);

    setInterceptsMouseClicks(false, false);
}

KnobBackground::~KnobBackground()
{
}

void KnobBackground::paint (juce::Graphics& g)
{
    // Render background image with error handling
    if (knobBackgroundImage.isValid())
    {
        g.drawImage(knobBackgroundImage, 0, 0, getWidth(), getHeight(), 0, 0, knobBackgroundImage.getWidth(), knobBackgroundImage.getHeight());
    }
}

void KnobBackground::resized()
{
    // No child components to layout
}
