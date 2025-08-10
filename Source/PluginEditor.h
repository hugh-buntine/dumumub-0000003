/*
  ==============================================================================

    PluginEditor.h
    
    DUMUMUB-0000003: Wavetable Synthesizer GUI
    
    Main editor interface featuring wavetable visualization, file import,
    waveform selection, and real-time parameter control.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "Canvas.h"
#include "CanvasBorder.h"
#include "SineButton.h"
#include "SawButton.h"
#include "SquareButton.h"
#include "TriangleButton.h"
#include "AudioButton.h"
#include "ImageButtonDUMUMUB.h"
#include "FileDisplay.h"
#include "StereoLight.h"
#include "HelpScreen.h"
#include "SliderLookAndFeel.h"
#include "VolumeDisplay.h"
#include "KnobBackground.h"

// Forward declarations
class LeftButton;
class RightButton;
class TitleButton;
class AddButton;
class ReplaceButton;
class FileDropZone;

//==============================================================================
/**
 * Main editor interface for DUMUMUB-0000003 wavetable synthesizer.
 * 
 * Provides real-time wavetable visualization, interactive editing,
 * file import capabilities, and comprehensive parameter control.
 */
class DUMUMUB003AudioProcessorEditor  : public juce::AudioProcessorEditor,
                                        public juce::Slider::Listener,
                                        public juce::Button::Listener
{
public:
    DUMUMUB003AudioProcessorEditor (DUMUMUB003AudioProcessor&);
    ~DUMUMUB003AudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider);
    void buttonClicked(juce::Button* button);

    //==============================================================================
    // Wavetable Interface
    void setWaveTableL(int index, float value);
    void setWaveTableR(int index, float value);
    std::array<float, 1024> getWaveTableL();
    std::array<float, 1024> getWaveTableR();

    // Channel State
    bool getLeftOn() { return leftOn; }
    bool getRightOn() { return rightOn; }

    // Color Scheme Access
    Colour getBlack() { return black; }
    Colour getWhite() { return white; }
    Colour getRed() { return red; }
    Colour getGreen() { return green; }
    Colour getBlue() { return blue; }

    // UI Control
    void toggleLeftLight() { leftOn = !leftOn; canvas.toggleLeft(leftOn); lightL.setOn(leftOn); audioProcessor.setLeftChannelState(leftOn); }
    void toggleRightLight() { rightOn = !rightOn; canvas.toggleRight(rightOn); lightR.setOn(rightOn); audioProcessor.setRightChannelState(rightOn); }

    void toggleHelp(bool value){ helpScreen.toggle(value); }
    void repaintCanvas(){ canvas.repaint(); }

    // File Display Management
    void setAudioFileName(String name){ fileDisplayAUDIO.setFileName(name); }
    void setImageFileName(String name){ fileDisplayIMAGE.setFileName(name); }

private:
    DUMUMUB003AudioProcessor& audioProcessor;

    //==============================================================================
    // UI Layout Constants
    int width = 1100;
    int height = 776;

    // Main Visualization
    Canvas canvas;
    CanvasBorder canvasBorder;

    // Navigation & Control Buttons
    std::unique_ptr<TitleButton> titleButton;
    std::unique_ptr<LeftButton> leftButton;
    std::unique_ptr<RightButton> rightButton;
    std::unique_ptr<AddButton> addButton;
    std::unique_ptr<ReplaceButton> replaceButton;
    
    // Waveform Selection Buttons
    SineButton sineButton;
    SawButton sawButton;
    SquareButton squareButton;
    TriangleButton triangleButton;
    AudioButton audioButton;
    ImageButtonDUMUMUB imageButton;

    // Help System
    HelpScreen helpScreen;

    // File Management
    FileDisplay fileDisplayAUDIO;
    FileDisplay fileDisplayIMAGE;
    std::unique_ptr<FileDropZone> fileDropZone;

    // Channel Indicators
    StereoLight lightL;
    StereoLight lightR;
    bool leftOn;
    bool rightOn;

    // Color Scheme
    Colour black = Colour::fromRGBA(20, 20, 0, 255);
    Colour white = Colour::fromRGBA(255, 255, 242, 255);
    Colour red = Colour::fromRGBA(255, 0, 0, 255);
    Colour green = Colour::fromRGBA(0, 255, 0, 255);
    Colour blue = Colour::fromRGBA(0, 0, 255, 255);

    // Background Graphics
    Image background;

    // Audio Controls
    Slider gainSlider;
    
    // Envelope Controls
    Slider attackSlider;
    Slider decaySlider;
    Slider sustainSlider;
    Slider releaseSlider;
    
    // Visual Components
    KnobBackground knobsBackground;
    SliderLookAndFeel sliderLookAndFeel;
    VolumeDisplay volumeDisplay;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DUMUMUB003AudioProcessorEditor)
};
