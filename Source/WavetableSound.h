/*
    ==============================================================================

        WavetableSound.h
        Created: 25 Feb 2025 12:07:14pm
        Author:  Hugh Buntine

        Wavetable sound class for DUMUMUB wavetable synthesizer.
        Defines sound object that responds to all MIDI notes and channels.

    ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
 * Wavetable Sound - Universal MIDI Sound Object
 * 
 * Simple sound class that accepts all MIDI notes and channels.
 * Required by JUCE synthesizer framework for polyphonic operation.
 * Works with WavetableVoice to create complete wavetable synthesis system.
 */
class WavetableSound : public juce::SynthesiserSound
{
public:
        WavetableSound() {} // Default constructor

        // Accept all MIDI note numbers for universal playback
        bool appliesToNote (int /*midiNoteNumber*/) override { return true; }
        
        // Accept all MIDI channels for universal compatibility
        bool appliesToChannel (int /*midiChannel*/) override { return true; }

private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WavetableSound)
};