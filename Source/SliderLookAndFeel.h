/*
  ==============================================================================

    SliderLookAndFeel.h
    Created: 5 Mar 2025 2:03:38pm
    Author:  Hugh Buntine

    Custom slider look and feel for DUMUMUB wavetable synthesizer.
    Provides image-based knobs with specialized ADSR graphics.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * Slider Look And Feel - Custom Image-Based Slider Rendering
 * 
 * Custom look and feel class that replaces default JUCE slider rendering
 * with image-based knobs. Provides specialized graphics for ADSR parameters
 * and general-purpose sliders throughout the synthesizer interface.
 */
class SliderLookAndFeel  : public LookAndFeel_V4
{
public:
    SliderLookAndFeel();
    ~SliderLookAndFeel() override;

    // Custom linear slider rendering with image-based thumb
    void drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                          float sliderPos, float minSliderPos, float maxSliderPos,
                          const Slider::SliderStyle, Slider& slider) override;

    // Custom rotary slider rendering with ADSR-specific knob images
    void drawRotarySlider(Graphics& g, int x, int y, int width, int height,
                          float sliderPosProportional, float rotaryStartAngle,
                          float rotaryEndAngle, Slider& slider) override;

private:
    // Slider and knob image assets
    Image sliderThumbImage;

    // ADSR-specific knob images
    Image attackKnobImage;
    Image decayKnobImage;
    Image sustainKnobImage;
    Image releaseKnobImage;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SliderLookAndFeel)
};