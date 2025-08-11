# dumumub-0000003 Wavetable Synthesizer Plugin

![Plugin Interface](https://img.shields.io/badge/JUCE-Audio%20Plugin-blue)
![Language](https://img.shields.io/badge/C++-17-red)
![Platform](https://img.shields.io/badge/Platform-macOS%20%7C%20Windows-green)
![Status](https://img.shields.io/badge/Status-Complete-brightgreen)

A sophisticated wavetable synthesizer audio plugin built with JUCE framework, featuring real-time wavetable editing, innovative image-to-audio conversion, and comprehensive MIDI synthesis capabilities.

## Plugin Interface

![DUMUMUB Interface](DEMO%20MEDIA/GUI.png)

*Professional-grade wavetable synthesizer interface with real-time editing capabilities*

## Project Overview

dumumub-0000003 is a professional-grade wavetable synthesizer plugin designed for digital audio workstations (DAWs). This project demonstrates advanced audio programming skills, real-time DSP processing, and modern C++ software architecture suitable for the audio technology industry.

**Duration:** February 2025 - March 2025  & August 2025
**Role:** Solo Developer  
**Technologies:** C++17, JUCE Framework, Audio Programming, DSP, MIDI

## Key Features

### Interactive Wavetable Editor
- **Visual Wavetable Canvas** - Real-time 1024-sample wavetable visualization and editing
- **Mouse-based Editing** - Direct wavetable manipulation with drag-and-draw functionality  
- **Dual-channel Support** - Independent left/right stereo channel wavetable editing
- **Live Audio Monitoring** - Real-time oscilloscope display during playback

![Drawing Wavetables](DEMO%20MEDIA/DRAWWAVE.gif)

*Manual wavetable drawing with mouse input for custom waveform creation*

![Channel Editing](DEMO%20MEDIA/MODIFYLEFTCHANNEL.gif)

*Independent left/right channel wavetable editing capabilities*

![Waveform Replacement](DEMO%20MEDIA/REPLACEWITHWAVES.gif)

*Real-time waveform addition and replacement system for dynamic and customisable sound design*

![Audio Playback](DEMO%20MEDIA/AUDIOPLAYBACK.gif)

*Audio playback and real-time monitoring with visual feedback*

### Advanced File Import System
- **Audio File Import** - Support for WAV, MP3, AIFF, FLAC formats with automatic wavetable conversion
- **Image-to-Wavetable Conversion** - Proprietary algorithm converting images (PNG, JPG) to audio waveforms
- **Drag-and-Drop Interface** - Intuitive file loading with visual feedback
- **State Persistence** - Complete session save/restore including file paths and parameters

![Image Import](DEMO%20MEDIA/IMAGEDROP.gif)

*Image-to-wavetable conversion with drag-and-drop interface*

![Audio Import](DEMO%20MEDIA/AUDIODROP.gif)

*Audio file drag-and-drop with automatic wavetable conversion*

![Adding Waveforms](DEMO%20MEDIA/ADDWAVE.gif)

*Ability to add common wave shapes to create a more cohesive sound*

### Core Synthesis Engine
- **Polyphonic Wavetable Synthesis** - 8-voice polyphony with independent stereo wavetables
- **Real-time ADSR Envelope** - Attack, Decay, Sustain, Release parameter control
- **MIDI Integration** - Full MIDI note and velocity support with proper frequency conversion
- **High-Quality Audio Processing** - 44.1kHz+ sample rate support with low-latency performance

![ADSR Control Demo](DEMO%20MEDIA/ADSR.gif)

*Real-time ADSR envelope parameter adjustment during playback*

### Professional UI/UX
- **Custom Graphics Engine** - Hand-designed interface with custom button states and hover effects
- **Responsive Controls** - Professional-grade sliders, knobs, and buttons with custom look-and-feel
- **Help System** - Integrated overlay help system for user guidance
- **Channel Indicators** - Visual stereo channel status with independent toggle controls

![Help System](DEMO%20MEDIA/HELP.gif)

*Built-in interactive help system with overlay guidance*

## Technical Implementation

### Architecture & Design Patterns
```cpp
// Core synthesis architecture using JUCE Synthesiser framework
class DUMUMUB003AudioProcessor : public juce::AudioProcessor
{
    // Polyphonic voice management
    Synthesiser synthesiser;                    // 8-voice polyphonic engine
    std::array<float, 1024> waveTableL;        // Left channel wavetable
    std::array<float, 1024> waveTableR;        // Right channel wavetable
    
    // Real-time parameter management
    juce::ADSR::Parameters adsrParams;         // Envelope parameters
    std::map<String, bool> selectedWaves;      // Waveform state management
};
```

### Real-time Audio Processing
- **Lock-free Audio Thread** - Proper real-time audio processing without blocking operations
- **Efficient Wavetable Interpolation** - Phase-accurate sample generation with wraparound
- **Multi-threaded Architecture** - Separate GUI and audio threads for optimal performance

### DSP Algorithms
- **Wavetable Synthesis** - Custom implementation with linear interpolation
- **Audio Analysis** - RMS level calculation for volume metering
- **Format Conversion** - Multi-format audio file reading and processing
- **Image Processing** - Pixel luminance analysis for waveform generation

### File I/O & Data Management
```cpp
// State persistence with XML serialization
void getStateInformation(juce::MemoryBlock& destData) override;
void setStateInformation(const void* data, int sizeInBytes) override;

// Audio file processing pipeline
void setAudioFromPath();                       // Load and process audio files
void fillAudioWavetableFromAudio();           // Convert audio to wavetable
void fillImageWavetableFromImage();           // Convert images to wavetables
```

## User Interface Features

### Component-Based Architecture
- **Modular UI Components** - Reusable button classes with inheritance hierarchy
- **Custom Graphics Rendering** - Optimized paint() methods with image caching
- **Event-Driven Design** - Proper listener patterns for user interaction
- **Responsive Layout** - Fixed-size design optimized for plugin windows

### Interactive Elements
- **Waveform Canvas** - 1024×500 pixel interactive drawing surface
- **Parameter Controls** - Gain slider (-36dB to +6dB) and ADSR knobs (0.0-1.0 range)
- **File Management** - Visual file name display with automatic updates
- **Channel Controls** - Independent L/R channel toggles with visual indicators

## Project Structure

```
DUMUMUB-0000003/
├── Source/
├── JuceLibraryCode/               # Generated JUCE framework code
└── Builds/                        # Platform-specific build outputs
    └── MacOSX/                    # Xcode project files
```

## Build System & Dependencies

### Development Environment
- **JUCE Framework** v7.x - Cross-platform audio application framework
- **Xcode** - macOS development and compilation
- **C++17 Standard** - Modern C++ features and best practices
- **Audio Plugin Formats** - VST3, AU, AAX, Standalone support

### Key Dependencies
```xml
<MODULES>
    <MODULE id="juce_audio_processors"/>    <!-- Plugin hosting -->
    <MODULE id="juce_audio_formats"/>       <!-- File I/O -->
    <MODULE id="juce_audio_utils"/>         <!-- DSP utilities -->
    <MODULE id="juce_gui_basics"/>          <!-- UI framework -->
    <MODULE id="juce_graphics"/>            <!-- 2D graphics -->
    <MODULE id="juce_dsp"/>                 <!-- Signal processing -->
</MODULES>
```

## Technical Challenges Solved

### 1. Real-time Wavetable Editing
**Challenge:** Enabling real-time wavetable modification without audio artifacts  
**Solution:** Implemented lock-free communication between GUI and audio threads using atomic operations and careful memory management

### 2. Image-to-Audio Conversion
**Challenge:** Converting 2D image data into meaningful audio waveforms  
**Solution:** Developed custom algorithm analyzing pixel luminance values column-wise, mapping vertical positions to amplitude values with normalization

### 3. Multi-format File Support  
**Challenge:** Supporting various audio and image file formats reliably  
**Solution:** Leveraged JUCE's AudioFormatManager with custom error handling and format validation

### 4. State Persistence
**Challenge:** Saving and restoring complete plugin state including file references  
**Solution:** Implemented XML-based serialization system preserving all parameters, file paths, and UI states

## Learning Outcomes & Skills Demonstrated

### Technical Skills
- **Audio Programming:** Real-time DSP, MIDI processing, plugin architecture
- **C++ Expertise:** Modern C++17, object-oriented design
- **Framework Proficiency:** JUCE audio framework, cross-platform development
- **Algorithm Development:** Custom DSP algorithms, image processing, data conversion
- **Performance Optimization:** Real-time constraints, memory management, CPU efficiency

### Software Engineering
- **Architecture Design:** Component-based systems, separation of concerns, modular design
- **User Interface:** Custom graphics, event handling, responsive design
- **File I/O:** Multiple format support, error handling, state management
- **Testing & Debugging:** Audio debugging techniques, performance profiling
- **Documentation:** Code documentation, technical specification writing

### Problem Solving
- **Creative Solutions:** Image-to-wavetable conversion algorithm development
- **Technical Constraints:** Real-time audio processing limitations and solutions
- **User Experience:** Intuitive interface design for complex audio tools
- **Cross-platform:** macOS/Windows compatibility considerations

## About the Developer

**Hugh Buntine** - Third-year Computing Student at The University of Melbourne
Passionate about audio technology, digital signal processing, and innovative music software development. Seeking graduate opportunities in audio technology, software engineering, or music technology companies.

**Technical Interests:** Real-time audio processing, machine learning for audio, spatial audio, plugin development

---

*This project demonstrates proficiency in C++, audio programming, DSP, and software engineering principles suitable for roles in audio technology companies, music software development, or general software engineering positions requiring high-performance, real-time systems expertise.*
