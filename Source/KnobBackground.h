/*
  ==============================================================================

    KnobBackground.h
    Created: 7 Aug 2025 11:49:49am
    Author:  Hugh Buntine

    Background image component for knob controls.
    Renders decorative backdrop while allowing mouse interactions to pass through.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
// Decorative background component for knob control areas
class KnobBackground  : public juce::Component
{
public:
    KnobBackground();
    ~KnobBackground() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KnobBackground)

    // Background image asset for knob visual styling
    Image knobBackgroundImage;
};
