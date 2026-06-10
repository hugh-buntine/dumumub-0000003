/*
  ==============================================================================

    PluginEditor.cpp
    Created: 28 Feb 2025 1:39:34pm
    Author:  Hugh Buntine

    Main plugin editor implementation for DUMUMUB wavetable synthesizer.
    Manages UI layout, component initialization, and user interaction handling.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "LeftButton.h"
#include "RightButton.h"
#include "TitleButton.h"
#include "AddButton.h"
#include "ReplaceButton.h"
#include "FileDropZone.h"

//==============================================================================
DUMUMUB003AudioProcessorEditor::DUMUMUB003AudioProcessorEditor (DUMUMUB003AudioProcessor& p)
    : AudioProcessorEditor (&p), 
      audioProcessor (p), 
      sineButton(p),
      sawButton(p),
      squareButton(p),
      triangleButton(p),
      audioButton(p),
      imageButton(p),
      volumeDisplay(p),
      canvas(p)
{
    // Set plugin window dimensions
    setSize (width, height);

    // Load and setup background image
    background = ImageFileFormat::loadFrom(BinaryData::BACKGROUND_png, BinaryData::BACKGROUND_pngSize);

    // Initialize main canvas and border components
    canvas.setBounds(38, 238, 1024, 500);
    addAndMakeVisible(canvas);
    canvasBorder.setBounds(0, 200, 1100, 576);
    addAndMakeVisible(canvasBorder);
    canvasBorder.toFront(true);

    // Setup UI buttons with state restoration
    titleButton = std::make_unique<TitleButton>(p);
    titleButton->setBounds(55, 10, 530, 80);
    addAndMakeVisible(titleButton.get());

    leftOn = audioProcessor.getLeftChannelState(); // Restore saved state
    leftButton = std::make_unique<LeftButton>(p);
    leftButton->setBounds(25, 90, 140, 60);
    addAndMakeVisible(leftButton.get());

    rightOn = audioProcessor.getRightChannelState(); // Restore saved state
    rightButton = std::make_unique<RightButton>(p);
    rightButton->setBounds(25, 150, 140, 60);
    addAndMakeVisible(rightButton.get());

    addButton = std::make_unique<AddButton>(p);
    addButton->setBounds(230, 90, 170, 60);
    addAndMakeVisible(addButton.get());

    replaceButton = std::make_unique<ReplaceButton>(p);
    replaceButton->setBounds(420, 90, 170, 60);
    addAndMakeVisible(replaceButton.get());

    // Initialize waveform selection buttons with saved states
    sineButton.setBounds(605, 90, 110, 60);
    addAndMakeVisible(sineButton);
    sineButton.setIsPressed(audioProcessor.getWaveformState("sine")); // Restore saved state

    sawButton.setBounds(725, 90, 110, 60);
    addAndMakeVisible(sawButton);
    sawButton.setIsPressed(audioProcessor.getWaveformState("saw")); // Restore saved state

    squareButton.setBounds(605, 150, 110, 60);
    addAndMakeVisible(squareButton);
    squareButton.setIsPressed(audioProcessor.getWaveformState("square")); // Restore saved state

    triangleButton.setBounds(725, 150, 110, 60);
    addAndMakeVisible(triangleButton);
    triangleButton.setIsPressed(audioProcessor.getWaveformState("triangle")); // Restore saved state

    audioButton.setBounds(845, 90, 110, 60);
    addAndMakeVisible(audioButton);
    audioButton.setIsPressed(audioProcessor.getWaveformState("audio")); // Restore saved state
    
    imageButton.setBounds(845, 150, 110, 60);
    addAndMakeVisible(imageButton);
    imageButton.setIsPressed(audioProcessor.getWaveformState("image")); // Restore saved state

    // Setup file display components with path restoration
    fileDisplayAUDIO.setBounds(955, 95, 110, 50);
    addAndMakeVisible(fileDisplayAUDIO);
    // Restore audio file name if path exists
    if (!audioProcessor.getAudioPath().isEmpty())
    {
        juce::File audioFile(audioProcessor.getAudioPath());
        setAudioFileName(audioFile.getFileName());
    }

    fileDisplayIMAGE.setBounds(955, 155, 110, 50);
    addAndMakeVisible(fileDisplayIMAGE);
    // Restore image file name if path exists
    if (!audioProcessor.getImagePath().isEmpty())
    {
        juce::File imageFile(audioProcessor.getImagePath());
        setImageFileName(imageFile.getFileName());
    }

    // Setup drag-and-drop file zone
    fileDropZone = std::make_unique<FileDropZone>(p);
    fileDropZone->setBounds(985, 10, 80, 90);
    addAndMakeVisible(fileDropZone.get());

    // Initialize stereo channel indicator lights
    lightL.setBounds(165, 95, 50, 50);
    addAndMakeVisible(lightL);

    lightR.setBounds(165, 155, 50, 50);
    addAndMakeVisible(lightR);

    // Initialize light states from saved channel states
    lightL.setOn(leftOn);
    lightR.setOn(rightOn);
    canvas.toggleLeft(leftOn);
    canvas.toggleRight(rightOn);

    // Setup help overlay screen
    helpScreen.setBounds(0, 0, 1100, 776);
    addAndMakeVisible(helpScreen);


    // Setup main gain slider with custom look and feel
    gainSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    gainSlider.setRange(-36, 6, 0.01); // Set the range with the correct interval
    gainSlider.setValue(juce::Decibels::gainToDecibels(audioProcessor.getGain())); // Set the initial value
    gainSlider.setBounds(605, 1, 345, 51);
    gainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    gainSlider.setLookAndFeel(&sliderLookAndFeel);
    gainSlider.addListener(this);
    addAndMakeVisible(gainSlider);

    // Get saved ADSR parameters from processor
    auto savedADSRParams = audioProcessor.getADSRParameters();

    // Setup ADSR envelope control knobs with state restoration
    attackSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    attackSlider.setRange(0.0, 1.0, 0.01);
    attackSlider.setValue(savedADSRParams.attack, juce::dontSendNotification); // Restore saved value
    attackSlider.setBounds(250, 145, 70, 70);
    attackSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    attackSlider.setLookAndFeel(&sliderLookAndFeel);
    attackSlider.addListener(this);
    attackSlider.setName("attackSlider");
    addAndMakeVisible(attackSlider);

    decaySlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    decaySlider.setRange(0.0, 1.0, 0.01);
    decaySlider.setValue(savedADSRParams.decay, juce::dontSendNotification); // Restore saved value
    decaySlider.setBounds(330, 145, 70, 70);
    decaySlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    decaySlider.setLookAndFeel(&sliderLookAndFeel);
    decaySlider.addListener(this);
    decaySlider.setName("decaySlider");
    addAndMakeVisible(decaySlider);

    sustainSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    sustainSlider.setRange(0.0, 1.0, 0.01);
    sustainSlider.setValue(savedADSRParams.sustain, juce::dontSendNotification); // Restore saved value
    sustainSlider.setBounds(410, 145, 70, 70);
    sustainSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    sustainSlider.setLookAndFeel(&sliderLookAndFeel);
    sustainSlider.addListener(this);
    sustainSlider.setName("sustainSlider");
    addAndMakeVisible(sustainSlider);

    releaseSlider.setSliderStyle(Slider::SliderStyle::RotaryVerticalDrag);
    releaseSlider.setRange(0.0, 1.0, 0.01);
    releaseSlider.setValue(savedADSRParams.release, juce::dontSendNotification); // Restore saved value
    releaseSlider.setBounds(490, 145, 70, 70);
    releaseSlider.setTextBoxStyle(Slider::NoTextBox, false, 0, 0);
    releaseSlider.setLookAndFeel(&sliderLookAndFeel);
    releaseSlider.addListener(this);
    releaseSlider.setName("releaseSlider");
    addAndMakeVisible(releaseSlider);

    // Add knob background overlay
    knobsBackground.setBounds(250, 145, 310, 70);
    addAndMakeVisible(knobsBackground);
    knobsBackground.setAlwaysOnTop(true);

    // Setup volume level display
    volumeDisplay.setBounds(610, 40, 335, 50);
    addAndMakeVisible(volumeDisplay);
}

DUMUMUB003AudioProcessorEditor::~DUMUMUB003AudioProcessorEditor()
{
    
    // Clean up ALL slider listeners and custom look and feel
    gainSlider.removeListener(this);
    gainSlider.setLookAndFeel(nullptr);
    
    attackSlider.removeListener(this);
    attackSlider.setLookAndFeel(nullptr);
    
    decaySlider.removeListener(this);
    decaySlider.setLookAndFeel(nullptr);
    
    sustainSlider.removeListener(this);
    sustainSlider.setLookAndFeel(nullptr);
    
    releaseSlider.removeListener(this);
    releaseSlider.setLookAndFeel(nullptr);
}

//==============================================================================
void DUMUMUB003AudioProcessorEditor::paint (juce::Graphics& g)
{
    // Render scaled background image to fit plugin window
    if (!background.isNull())
    {
        g.drawImage(background, 0, 0, 1100, 776, 0, 0, background.getWidth(), background.getHeight());
    }
}

void DUMUMUB003AudioProcessorEditor::resized()
{
    // Component layout handled in constructor
}

void DUMUMUB003AudioProcessorEditor::sliderValueChanged(juce::Slider* slider) 
{
    // Handle gain slider changes with dB to linear conversion
    if (slider == &gainSlider)
    {
        if (slider->getValue() == -36)
        {
            audioProcessor.setGain(0.0);
            return;
        }
        else
        {
            float gain = juce::Decibels::decibelsToGain(slider->getValue());
            audioProcessor.setGain(gain);
        }
    }
    
    // Handle ADSR parameter changes
    if (slider == &attackSlider || slider == &decaySlider || slider == &sustainSlider || slider == &releaseSlider)
    {
        juce::ADSR::Parameters params;
        params.attack = attackSlider.getValue();
        params.decay = decaySlider.getValue();
        params.sustain = sustainSlider.getValue();
        params.release = releaseSlider.getValue();

        audioProcessor.setADSRParameters(params);
    }
}

void DUMUMUB003AudioProcessorEditor::buttonClicked(juce::Button* button)
{
    // Button click handling delegated to individual button components
}

// Wavetable manipulation interface methods
void DUMUMUB003AudioProcessorEditor::setWaveTableL(int index, float value)
{
    // Forward left channel wavetable changes to processor
    audioProcessor.setWaveTableL(index, value);
}

void DUMUMUB003AudioProcessorEditor::setWaveTableR(int index, float value)
{
    // Forward right channel wavetable changes to processor
    audioProcessor.setWaveTableR(index, value);
}

std::array<float, 1024>  DUMUMUB003AudioProcessorEditor::getWaveTableL()
{
    // Retrieve left channel wavetable data from processor
    return audioProcessor.getWaveTableL();
}

std::array<float, 1024>  DUMUMUB003AudioProcessorEditor::getWaveTableR()
{
    // Retrieve right channel wavetable data from processor
    return audioProcessor.getWaveTableR();
}
