/*
  ==============================================================================

    HelpScreen.cpp
    Created: 3 Mar 2025 2:56:02pm
    Author:  Hugh Buntine

    Overlay help screen component with fade in/out functionality.
    Displays help information with opacity-based visibility control.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "HelpScreen.h"

//==============================================================================
HelpScreen::HelpScreen()
{
    // Initialize visibility state and load help image
    isOn = false;

    picture = ImageFileFormat::loadFrom(BinaryData::HELP_png, BinaryData::HELP_pngSize);
}

HelpScreen::~HelpScreen()
{
}

void HelpScreen::paint (juce::Graphics& g)
{
    // Set opacity based on visibility state
    if (isOn)
    {
        g.setOpacity(1);
    }
    else
    {
        g.setOpacity(0);
    }
    g.drawImage(picture, 0, 0, getWidth(), getHeight(), 0, 0, picture.getWidth(), picture.getHeight());
}

void HelpScreen::resized()
{
    // No child components to layout
}