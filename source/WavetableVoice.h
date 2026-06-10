/*
  ==============================================================================

    WavetableVoice.h
    Created: 25 Feb 2025 12:05:10pm
    Author:  Hugh Buntine

    Polyphonic synthesizer voice for wavetable playback with ADSR envelope.
    Supports independent stereo wavetables and real-time parameter control.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "WavetableSound.h"
#include "PluginProcessor.h"
#include <algorithm>
#include <cmath>

//==============================================================================
/**
 * Individual synthesizer voice for polyphonic wavetable playback.
 * 
 * Features:
 * - Independent stereo wavetable playback
 * - MIDI note frequency conversion
 * - ADSR envelope processing
 * - Real-time gain and output volume control
 */
class WavetableVoice : public juce::SynthesiserVoice
{
public:
    WavetableVoice(DUMUMUB003AudioProcessor& p)
        : leftPhase(0.0f), rightPhase(0.0f), leftPhaseIncrement(0.0f), rightPhaseIncrement(0.0f), audioProcessor(p)
    {
    }

    // Check if this voice can play the given sound type
    bool canPlaySound (juce::SynthesiserSound* sound) override
    {
        return dynamic_cast<WavetableSound*> (sound) != nullptr;
    }

    // Initialize voice parameters when MIDI note starts
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound*, int /*currentPitchWheelPosition*/) override
    {
        adsr.setParameters(adsrParams);
        adsr.noteOn();

        // Convert MIDI note to frequency and calculate phase increment
        auto frequency = juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber);
        leftPhaseIncrement = frequency / getSampleRate() * wavetableSize;
        rightPhaseIncrement = frequency / getSampleRate() * wavetableSize;
        leftPhase = 0.0f;
        rightPhase = 0.0f;
        level = velocity;
    }

    // Handle MIDI note release
    void stopNote (float /*velocity*/, bool allowTailOff) override
    {
        adsr.noteOff();
    
        if (!allowTailOff)
            clearCurrentNote();
    }
    
    

    // MIDI controller handling (not implemented)
    void pitchWheelMoved (int /*newPitchWheelValue*/) override {}
    void controllerMoved (int /*controllerNumber*/, int /*newControllerValue*/) override {}
    
    // Check if voice is currently active
    bool isVoiceActive() const override
    {
        return adsr.isActive();
    }
    
    // Update ADSR envelope parameters
    void setADSRParameters(const juce::ADSR::Parameters& params)
    {
        adsrParams = params;
    }

    // Core audio rendering with stereo wavetable playback and ADSR envelope
    void renderNextBlock (juce::AudioBuffer<float>& outputBuffer, int startSample, int numSamples) override
    {
        float gain = audioProcessor.getGain();
        float outputVolume = audioProcessor.getOutputVolume(); // Master volume control

        if (gain <= 0.0f || level == 0.0f)
            return;

        auto* leftChannel = outputBuffer.getWritePointer(0, startSample);
        auto* rightChannel = outputBuffer.getWritePointer(1, startSample);

        // Get wavetable references
        const auto& leftWavetable = audioProcessor.waveTableL;
        const auto& rightWavetable = audioProcessor.waveTableR;

        // Generate audio samples with wavetable interpolation and envelope
        for (int sample = 0; sample < numSamples; ++sample)
        {
            auto leftSample = leftWavetable[(int)leftPhase];
            auto rightSample = rightWavetable[(int)rightPhase];

            auto envValue = adsr.getNextSample();

            // Apply velocity, gain, volume, and envelope to output
            leftChannel[sample] += leftSample * level * gain * outputVolume * envValue;
            rightChannel[sample] += rightSample * level * gain * outputVolume * envValue;

            // Advance wavetable phase with wraparound
            leftPhase += leftPhaseIncrement;
            if (leftPhase >= wavetableSize)
                leftPhase -= wavetableSize;

            rightPhase += rightPhaseIncrement;
            if (rightPhase >= wavetableSize)
                rightPhase -= wavetableSize;
        }

        // Clean up voice when envelope completes
        if (!adsr.isActive())
        clearCurrentNote();
    }

private:
    // Wavetable playback state
    float leftPhase;
    float rightPhase;
    float leftPhaseIncrement;
    float rightPhaseIncrement;
    int wavetableSize = 1024;
    float level = 1.0f;

    // ADSR envelope processing
    ADSR adsr;
    ADSR::Parameters adsrParams;

    // Reference to audio processor for wavetable data
    DUMUMUB003AudioProcessor& audioProcessor;



    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableVoice)
};
