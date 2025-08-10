/*
  ==============================================================================

    PluginProcessor.h
    
    DUMUMUB-0000003: Wavetable Synthesizer
    
    Multi-waveform wavetable synthesizer with real-time editing, file import,
    and innovative image-to-wavetable conversion capabilities.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * Main audio processor for DUMUMUB-0000003 wavetable synthesizer.
 * 
 * Features:
 * - Polyphonic wavetable synthesis with ADSR envelopes
 * - Multiple waveform types (sine, square, triangle, sawtooth)
 * - Audio file import and wavetable generation
 * - Image-to-wavetable conversion
 * - Real-time wavetable editing
 * - Independent stereo channel processing
 * - Comprehensive state persistence
 */

class DUMUMUB003AudioProcessor  : public juce::AudioProcessor
{
public:
    //==============================================================================
    DUMUMUB003AudioProcessor();
    ~DUMUMUB003AudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    //==============================================================================
    // Audio Parameters
    void setGain(float newGain);
    float getGain(){ return gain; };
    float getOutputVolume(){ return outputVolume; };

    // Wavetable Access
    void setWaveTableL(int index, float value);
    void setWaveTableR(int index, float value);
    std::array<float, 1024> getWaveTableL();
    std::array<float, 1024> getWaveTableR();

    // Waveform Generation
    void fillSineWavetable();
    void fillSquareWavetable();
    void fillTriangleWavetable();
    void fillSawtoothWavetable();
    void fillAudioWavetables();
    void fillImageWavetable();

    // Wavetable Operations
    void copyWaveTableToL(std::array<float, 1024> waveTable);
    void copyWaveTableToR(std::array<float, 1024> waveTable);
    void addWaveTableToL();
    void addWaveTableToR();
    void replaceTableToL();
    void replaceTableToR();

    // GUI State Management
    void setWaveformType(String waveform, bool value);

    // File Management
    void setAudioPath(String path){ audioPath = path; }
    void setImagePath(String path){ imagePath = path; }
    String getAudioPath(){ return audioPath; }
    String getImagePath(){ return imagePath; }
    void setAudioFromPath();
    void setImageFromPath();
    void fillAudioWavetableFromAudio();
    void fillImageWavetableFromImage();

    // Utility Functions
    void normalizeWave(std::array<float, 1024>& waveTable);

    // Audio Analysis
    float getLoudnessL(){ return loudnessL; }
    float getLoudnessR(){ return loudnessR; }

    // Buffer Conversion
    std::array<float, 1024> bufferToWaveTableL(AudioBuffer<float>& buffer);
    std::array<float, 1024> bufferToWaveTableR(AudioBuffer<float>& buffer);

    std::array<float, 1024> getCurrentBufferL(){ return currentWaveBufferTableL; }
    std::array<float, 1024> getCurrentBufferR(){ return currentWaveBufferTableR; }

    // ADSR Control
    void setADSRParameters(const juce::ADSR::Parameters& params);

    // GUI state access methods
    bool getWaveformState(const String& waveform) { return selectedWaves[waveform]; }
    juce::ADSR::Parameters getADSRParameters() { return adsrParams; }
    
    // Channel State Management
    void setLeftChannelState(bool state) { leftChannelOn = state; }
    void setRightChannelState(bool state) { rightChannelOn = state; }
    bool getLeftChannelState() { return leftChannelOn; }
    bool getRightChannelState() { return rightChannelOn; }

    // Public wavetable data for real-time access
    std::array<float, 1024> waveTableL; 
    std::array<float, 1024> waveTableR;

private:
    //==============================================================================
    // Audio Parameters
    float gain;
    float outputVolume;

    // Waveform Templates
    std::array<float, 1024> sineWave;
    std::array<float, 1024> squareWave;
    std::array<float, 1024> triangleWave;
    std::array<float, 1024> sawtoothWave;
    std::array<float, 1024> audioWaveL;
    std::array<float, 1024> audioWaveR;
    std::array<float, 1024> imageWave;

    // File Import Data
    String audioPath;
    String imagePath;
    AudioBuffer<float> droppedAudio;
    Image droppedImage;

    // Synthesis Engine
    Synthesiser synthesiser;

    // GUI State
    std::map<String, bool> selectedWaves;

    // Audio Analysis
    float loudnessL;
    float loudnessR;

    // Real-time Buffer Processing
    std::array<float, 1024> currentWaveBufferTableL;
    std::array<float, 1024> currentWaveBufferTableR;

    // Initialization State
    bool waveTablesInitialized = false;

    // Envelope Parameters
    juce::ADSR::Parameters adsrParams;
    
    // Channel State
    bool leftChannelOn = true;
    bool rightChannelOn = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DUMUMUB003AudioProcessor)
};
