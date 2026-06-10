/*
  ==============================================================================

    SliderLookAndFeel.cpp
    Created: 5 Mar 2025 2:03:38pm
    Author:  Hugh Buntine

    Custom slider look and feel implementation for DUMUMUB wavetable synthesizer.
    Provides image-based knobs and sliders with ADSR-specific graphics.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "SliderLookAndFeel.h"

SliderLookAndFeel::SliderLookAndFeel()
{
  // Load knob images from binary resources
  sliderThumbImage = ImageFileFormat::loadFrom(BinaryData::KNOB_png, BinaryData::KNOB_pngSize);
  attackKnobImage = ImageFileFormat::loadFrom(BinaryData::ATTACK_KNOB_png, BinaryData::ATTACK_KNOB_pngSize);
  decayKnobImage = ImageFileFormat::loadFrom(BinaryData::DECAY_KNOB_png, BinaryData::DECAY_KNOB_pngSize);
  sustainKnobImage = ImageFileFormat::loadFrom(BinaryData::SUSTAIN_KNOB_png, BinaryData::SUSTAIN_KNOB_pngSize);
  releaseKnobImage = ImageFileFormat::loadFrom(BinaryData::RELEASE_KNOB_png, BinaryData::RELEASE_KNOB_pngSize);
}

SliderLookAndFeel::~SliderLookAndFeel() {}

// Custom linear slider rendering with image-based thumb
void SliderLookAndFeel::drawLinearSlider(Graphics& g, int x, int y, int width, int height,
                        float sliderPos, float minSliderPos, float maxSliderPos,
                        const Slider::SliderStyle, Slider& slider)
{
  // Scale factor for image sizing (image is oversized for clarity)
  int scaleFactor = 6;

  const int thumbWidth = sliderThumbImage.getWidth()  / scaleFactor;
  const int thumbHeight = sliderThumbImage.getHeight()  / scaleFactor;
  const int thumbX = static_cast<int>(sliderPos) - (thumbWidth / 2);
  const int thumbY = y + (height / 2) - (thumbHeight / 2);

  // Draw scaled thumb image at calculated position
  g.drawImage(sliderThumbImage, thumbX, thumbY, thumbWidth, thumbHeight, 0, 0, thumbWidth * scaleFactor, thumbHeight * scaleFactor);
}

// Custom rotary slider rendering with ADSR-specific knob images
void SliderLookAndFeel::drawRotarySlider(Graphics& g, int x, int y, int width, int height,
  float sliderPosProportional, float rotaryStartAngle,
  float rotaryEndAngle, Slider& slider)
{
Image knobImage;

// Select appropriate knob image based on slider name
auto sliderName = slider.getName();

if (sliderName == "attackSlider")
knobImage = attackKnobImage;
else if (sliderName == "decaySlider")
knobImage = decayKnobImage;
else if (sliderName == "sustainSlider")
knobImage = sustainKnobImage;
else if (sliderName == "releaseSlider")
knobImage = releaseKnobImage;
else
knobImage = sliderThumbImage;

if (!knobImage.isValid())
return;

// Calculate rotation angle based on slider position
const float rotationAngle = sliderPosProportional * (3.0f * MathConstants<float>::pi / 2.0f);

// Scale and position knob image
const float scale = 0.3f; // Adjust based on desired knob size
const int drawWidth = static_cast<int>(knobImage.getWidth() * scale);
const int drawHeight = static_cast<int>(knobImage.getHeight() * scale);
const int drawX = x + (width - drawWidth) / 2;
const int drawY = y + (height - drawHeight) / 2;

// Draw rotated and scaled knob image
g.drawImageTransformed(knobImage,
    AffineTransform::rotation(rotationAngle, knobImage.getWidth() / 2.0f, knobImage.getHeight() / 2.0f)
        .scaled(scale)
        .translated((float)drawX, (float)drawY));

}




