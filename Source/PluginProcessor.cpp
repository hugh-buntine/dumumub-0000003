	/*
  ==============================================================================

    PluginProcessor.cpp
    Created: 28 Feb 2025 1:39:34pm
    Author:  Hugh Buntine

    Core audio processor implementation for DUMUMUB wavetable synthesizer.
    Handles audio processing, state management, and wavetable synthesis.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "WavetableVoice.h"
#include "WavetableSound.h"

//==============================================================================
DUMUMUB003AudioProcessor::DUMUMUB003AudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // Initialize audio parameters
    outputVolume = 0.4f;

    gain = 1.0f;

    loudnessL = 0.0f;
    loudnessR = 0.0f;

    // Initialize wavetable arrays
    waveTableL.fill(0.0);
    waveTableR.fill(0.0);

    currentWaveBufferTableL.fill(0.0);
    currentWaveBufferTableR.fill(0.0);

    // Setup polyphonic synthesizer with 8 voices
    for (int i = 0; i < 8; ++i) // Add 8 voices for polyphony
            synthesiser.addVoice(new WavetableVoice(*this));
    synthesiser.addSound(new WavetableSound());

    // Initialize ADSR parameters with default values
    adsrParams.attack = 0.1f;
    adsrParams.decay = 0.1f;
    adsrParams.sustain = 1.0f;
    adsrParams.release = 0.1f;

    // Initialize selected waves map with default values
    selectedWaves["sine"] = false;
    selectedWaves["square"] = false;
    selectedWaves["triangle"] = false;
    selectedWaves["saw"] = false;
    selectedWaves["audio"] = false;
    selectedWaves["image"] = false;
}

DUMUMUB003AudioProcessor::~DUMUMUB003AudioProcessor()
{
}

//==============================================================================
const juce::String DUMUMUB003AudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DUMUMUB003AudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DUMUMUB003AudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool DUMUMUB003AudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double DUMUMUB003AudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DUMUMUB003AudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DUMUMUB003AudioProcessor::getCurrentProgram()
{
    return 0;
}

void DUMUMUB003AudioProcessor::setCurrentProgram (int index)
{
}

const juce::String DUMUMUB003AudioProcessor::getProgramName (int index)
{
    return {};
}

void DUMUMUB003AudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void DUMUMUB003AudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Initialize wavetables on first playback preparation
    if (!waveTablesInitialized)
    {
        fillSineWavetable();
        fillSquareWavetable();
        fillTriangleWavetable();
        fillSawtoothWavetable();
        fillAudioWavetables();
        fillImageWavetable();

        // Set default sine wave for both channels
        copyWaveTableToL(sineWave);
        copyWaveTableToR(sineWave);

        waveTablesInitialized = true;
    }

    // Configure synthesizer sample rate
    synthesiser.setCurrentPlaybackSampleRate(sampleRate);
}

void DUMUMUB003AudioProcessor::releaseResources()
{
    // Free up resources when playback stops
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DUMUMUB003AudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DUMUMUB003AudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Clear unused output channels
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // Render synthesizer output
    synthesiser.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // Calculate RMS loudness levels for both channels
    float newLoudnessL = 0.0f;
    float newLoudnessR = 0.0f;
    for (int channel = 0; channel < totalNumOutputChannels; ++channel)
    {
        for (int sample = 0; sample < buffer.getNumSamples(); ++sample)
        {
            if (channel == 0)
            {
                newLoudnessL += buffer.getSample(channel, sample) * buffer.getSample(channel, sample);
            }
            else if (channel == 1)
            {
                newLoudnessR += buffer.getSample(channel, sample) * buffer.getSample(channel, sample);
            }
        }
    }
    newLoudnessL = std::sqrt(newLoudnessL / buffer.getNumSamples());
    newLoudnessR = std::sqrt(newLoudnessR / buffer.getNumSamples());
    loudnessL = newLoudnessL;
    loudnessR = newLoudnessR;

    // Update current wavetable display from audio buffer
    currentWaveBufferTableL = bufferToWaveTableL(buffer);
    currentWaveBufferTableR = bufferToWaveTableR(buffer);
}

//==============================================================================
bool DUMUMUB003AudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* DUMUMUB003AudioProcessor::createEditor()
{
    return new DUMUMUB003AudioProcessorEditor (*this);
}

//==============================================================================
void DUMUMUB003AudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // Create XML structure for complete state persistence
    std::unique_ptr<juce::XmlElement> xml (new juce::XmlElement ("DUMUMUB003State"));

    // Audio parameters
    xml->setAttribute ("gain", gain);

    // Wavetable data - all 1024 samples for both channels
    for (int i = 0; i < waveTableL.size(); ++i)
    {
        xml->setAttribute ("waveTableL_" + juce::String (i), waveTableL[i]);
        xml->setAttribute ("waveTableR_" + juce::String (i), waveTableR[i]);
    }

    // GUI state - button selections
    for (const auto& wave : selectedWaves)
    {
        xml->setAttribute ("selectedWave_" + wave.first, wave.second);
    }

    // Save file paths (GUI state)
    xml->setAttribute ("audioPath", audioPath);
    xml->setAttribute ("imagePath", imagePath);

    // Save ADSR parameters
    xml->setAttribute ("attack", adsrParams.attack);
    xml->setAttribute ("decay", adsrParams.decay);
    xml->setAttribute ("sustain", adsrParams.sustain);
    xml->setAttribute ("release", adsrParams.release);

    // Save channel states
    xml->setAttribute ("leftChannelOn", leftChannelOn);
    xml->setAttribute ("rightChannelOn", rightChannelOn);

    // Convert the XML to a string and copy it to the memory block
    copyXmlToBinary (*xml, destData);
}

void DUMUMUB003AudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // Convert the memory block to an XML element
    std::unique_ptr<juce::XmlElement> xml (getXmlFromBinary (data, sizeInBytes));

    if (xml != nullptr && xml->hasTagName ("DUMUMUB003State"))
    {
        // Restore the gain value
        gain = xml->getDoubleAttribute ("gain", 1.0);

        // Restore the wave tables
        for (int i = 0; i < waveTableL.size(); ++i)
        {
            waveTableL[i] = xml->getDoubleAttribute ("waveTableL_" + juce::String (i), 0.0);
            waveTableR[i] = xml->getDoubleAttribute ("waveTableR_" + juce::String (i), 0.0);
        }
        // Restore selected waveforms (GUI state)
        selectedWaves["sine"] = xml->getBoolAttribute ("selectedWave_sine", false);
        selectedWaves["square"] = xml->getBoolAttribute ("selectedWave_square", false);
        selectedWaves["triangle"] = xml->getBoolAttribute ("selectedWave_triangle", false);
        selectedWaves["saw"] = xml->getBoolAttribute ("selectedWave_saw", false);
        selectedWaves["audio"] = xml->getBoolAttribute ("selectedWave_audio", false);
        selectedWaves["image"] = xml->getBoolAttribute ("selectedWave_image", false);

        // Restore file paths (GUI state)
        audioPath = xml->getStringAttribute ("audioPath", "");
        imagePath = xml->getStringAttribute ("imagePath", "");

        // Restore ADSR parameters
        adsrParams.attack = xml->getDoubleAttribute ("attack", 0.1f);
        adsrParams.decay = xml->getDoubleAttribute ("decay", 0.1f);
        adsrParams.sustain = xml->getDoubleAttribute ("sustain", 1.0f);
        adsrParams.release = xml->getDoubleAttribute ("release", 0.1f);

        // Restore channel states
        leftChannelOn = xml->getBoolAttribute ("leftChannelOn", true);
        rightChannelOn = xml->getBoolAttribute ("rightChannelOn", true);

        // Apply the loaded state
        copyWaveTableToL(waveTableL);
        copyWaveTableToR(waveTableR);

        // Apply the loaded gain value
        setGain(gain);

        // Reload audio and image files if paths exist
        if (!audioPath.isEmpty())
        {
            setAudioFromPath();
        }
        
        if (!imagePath.isEmpty())
        {
            setImageFromPath();
        }
    }
}

//==============================================================================
// Wavetable access interface methods
void DUMUMUB003AudioProcessor::setWaveTableL(int index, float value)
{
    waveTableL[index] = value;
}
void DUMUMUB003AudioProcessor::setWaveTableR(int index, float value)
{
    waveTableR[index] = value;
}

std::array<float, 1024> DUMUMUB003AudioProcessor::getWaveTableL()
{
    return waveTableL;
}

std::array<float, 1024> DUMUMUB003AudioProcessor::getWaveTableR()
{
    return waveTableR;
}

// Wavetable generation methods
void DUMUMUB003AudioProcessor::fillSineWavetable()
{
    const int wavetableSize = sineWave.size();
    const double amplitude = 1.0;

    if (wavetableSize <= 0)
    {
        return;
    }

    // Generate sine wave with inverted polarity
    for (int i = 0; i < wavetableSize; ++i)
    {
        double phase = 2.0 * juce::MathConstants<double>::pi * i / wavetableSize;
        sineWave[i] = -amplitude * std::sin(phase);
    }
}

void DUMUMUB003AudioProcessor::fillSquareWavetable()
{
    const int wavetableSize = squareWave.size();
    const double amplitude = 1.0;

    if (wavetableSize <= 0)
    {
        return;
    }

    // Generate square wave based on sine wave polarity
    for (int i = 0; i < wavetableSize; ++i)
    {
        double phase = 2.0 * juce::MathConstants<double>::pi * i / wavetableSize;
        squareWave[i] = (std::sin(phase) >= 0.0) ? -amplitude : amplitude;
    }
}

void DUMUMUB003AudioProcessor::fillTriangleWavetable() 
{
    const int wavetableSize = triangleWave.size();
    const double amplitude = 1.0;

    if (wavetableSize <= 0)
    {
        return;
    }

    // Generate triangle wave using inverse sine approximation
    for (int i = 0; i < wavetableSize; ++i)
    {
        double phase = 2.0 * juce::MathConstants<double>::pi * i / wavetableSize;
        triangleWave[i] = -amplitude * (2.0 / juce::MathConstants<double>::pi) * std::asin(std::sin(phase));
    }
}

void DUMUMUB003AudioProcessor::fillSawtoothWavetable()
{
    const int wavetableSize = sawtoothWave.size();
    const double amplitude = 1.0;

    if (wavetableSize <= 0)
    {
        return;
    }

    // Generate linear sawtooth ramp
    for (int i = 0; i < wavetableSize; ++i)
    {
        double phase = 2.0 * juce::MathConstants<double>::pi * i / wavetableSize;
        sawtoothWave[i] = amplitude * (2.0 * (i / static_cast<double>(wavetableSize)) - 1.0);
    }
}

void DUMUMUB003AudioProcessor::fillAudioWavetables()
{
    const int wavetableLSize = audioWaveL.size();

    if (wavetableLSize <= 0)
    {
        return;
    }

    // Initialize audio wavetables with silence
    for (int i = 0; i < wavetableLSize; ++i)
    {
        audioWaveL[i] = 0;
    }

    const int wavetableRSize = audioWaveR.size();

    if (wavetableRSize <= 0)
    {
        return;
    }

    for (int i = 0; i < wavetableLSize; ++i)
    {
        audioWaveR[i] = 0;
    }
}

void DUMUMUB003AudioProcessor::fillImageWavetable()
{
    const int wavetableSize = imageWave.size();

    if (wavetableSize <= 0)
    {
        return;
    }

    // Initialize image wavetable with silence
    for (int i = 0; i < wavetableSize; ++i)
    {
        imageWave[i] = 0;
    }
}

// Wavetable manipulation methods
void DUMUMUB003AudioProcessor::copyWaveTableToL(std::array<float, 1024> source)
{
    for (int i = 0; i < waveTableL.size(); i++)
    {
        waveTableL[i] = source[i];
    }
}

void DUMUMUB003AudioProcessor::copyWaveTableToR(std::array<float, 1024> source)
{
    for (int i = 0; i < waveTableR.size(); i++)
    {
        waveTableR[i] = source[i];
    }
}

// Waveform mixing methods - add selected waveforms to existing content
void DUMUMUB003AudioProcessor::addWaveTableToL()
{
    std::array<float, 1024> waveTable; for (int i = 0; i < waveTableL.size(); i++) { waveTable[i] = 0;}
    int waveCount = 0;
   
    // Accumulate selected waveforms
    if (selectedWaves["sine"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += sineWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["square"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += squareWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["triangle"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += triangleWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["saw"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += sawtoothWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["audio"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += audioWaveL[i];
        }
        waveCount++;
    }
    if (selectedWaves["image"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += imageWave[i];
        }
        waveCount++;
    }

    if (waveCount == 0)
    {
        return;
    }
    else
    {
        // Blend new waveforms with existing content (50/50 mix)
        for (int i = 0; i < waveTableL.size(); i++)
        {
            waveTableL[i] = (waveTableL[i] + (waveTable[i] / waveCount)) / 2;
        }
    }
}

void DUMUMUB003AudioProcessor::addWaveTableToR()
{
    std::array<float, 1024> waveTable; for (int i = 0; i < waveTableR.size(); i++) { waveTable[i] = 0;}
    int waveCount = 0;
   
    // Accumulate selected waveforms
    if (selectedWaves["sine"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += sineWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["square"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += squareWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["triangle"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += triangleWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["saw"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += sawtoothWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["audio"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += audioWaveR[i];
        }
        waveCount++;
    }
    if (selectedWaves["image"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += imageWave[i];
        }
        waveCount++;
    }

    if (waveCount == 0)
    {
        return;
    }
    else
    {
        // Blend new waveforms with existing content (50/50 mix)
        for (int i = 0; i < waveTableR.size(); i++)
        {
            waveTableR[i] = (waveTableR[i] + (waveTable[i] / waveCount)) / 2;
        }
    }
}

// Waveform replacement methods - replace existing content with selected waveforms
void DUMUMUB003AudioProcessor::replaceTableToL()
{
    std::array<float, 1024> waveTable; for (int i = 0; i < waveTableL.size(); i++) { waveTable[i] = 0;}
    int waveCount = 0;
   
    // Accumulate selected waveforms
    if (selectedWaves["sine"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += sineWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["square"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += squareWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["triangle"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += triangleWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["saw"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += sawtoothWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["audio"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += audioWaveL[i];
        }
        waveCount++;
    }
    if (selectedWaves["image"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += imageWave[i];
        }
        waveCount++;
    }

    if (waveCount == 0)
    {
        return;
    }
    else
    {
        // Replace existing content with averaged selected waveforms
        for (int i = 0; i < waveTableL.size(); i++)
        {
            waveTableL[i] = waveTable[i] / waveCount;
        }
    }
}

void DUMUMUB003AudioProcessor::replaceTableToR()
{
    std::array<float, 1024> waveTable; for (int i = 0; i < waveTableR.size(); i++) { waveTable[i] = 0;}
    int waveCount = 0;
   
    // Accumulate selected waveforms
    if (selectedWaves["sine"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += sineWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["square"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += squareWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["triangle"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += triangleWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["saw"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += sawtoothWave[i];
        }
        waveCount++;
    }
    if (selectedWaves["audio"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += audioWaveR[i];
        }
        waveCount++;
    }
    if (selectedWaves["image"])
    {
        for (int i = 0; i < waveTable.size(); i++)
        {
            waveTable[i] += imageWave[i];
        }
        waveCount++;
    }

    if (waveCount == 0)
    {
        return;
    }
    else
    {
        // Replace existing content with averaged selected waveforms
        for (int i = 0; i < waveTableR.size(); i++)
        {
            waveTableR[i] = waveTable[i] / waveCount;
        }
    }
}

// Audio parameter management
void DUMUMUB003AudioProcessor::setGain(float newGain)
{
    gain = newGain;
}

void DUMUMUB003AudioProcessor::setWaveformType(String waveform, bool value)
{
    selectedWaves[waveform] = value;
}

// File loading and processing methods
void DUMUMUB003AudioProcessor::setAudioFromPath()
{
    File audioFile(audioPath);
    AudioFormatManager formatManager;
    formatManager.registerBasicFormats();
    std::unique_ptr<AudioFormatReader> audioReader(formatManager.createReaderFor(audioFile));
    if (audioReader.get() != nullptr)
    {
        int numChannels = audioReader->numChannels;
        int numSamples = audioReader->lengthInSamples;
        AudioBuffer<float> audioBuffer(numChannels, numSamples);
        audioReader->read(&audioBuffer, 0, numSamples, 0, true, true);
        
        droppedAudio = audioBuffer;
        fillAudioWavetableFromAudio();
        
    }
}

void DUMUMUB003AudioProcessor::setImageFromPath()
{
    File imageFile(imagePath);
    Image image = ImageFileFormat::loadFrom(imageFile);
    if (!image.isNull())
    {
        droppedImage = image;
        fillImageWavetableFromImage();
    }
}

void DUMUMUB003AudioProcessor::fillAudioWavetableFromAudio()
{
    const int wavetableLSize = audioWaveL.size();
    const int wavetableRSize = audioWaveR.size();
    const int numSamples = droppedAudio.getNumSamples();
    const int numChannels = droppedAudio.getNumChannels();
    const int numSamplesPerChannel = numSamples / numChannels;

    if (wavetableLSize <= 0 || wavetableRSize <= 0)
    {
        return;
    }

    // Clear existing audio wavetables
    for (int i = 0; i < wavetableLSize; ++i)
    {
        audioWaveL[i] = 0;
    }
    for (int i = 0; i < wavetableRSize; ++i)
    {
        audioWaveR[i] = 0;
    }

    // Extract 1024 samples from the middle of the audio file
    int middle = numSamplesPerChannel / 2;

    for (int i = 0; i < audioWaveL.size(); ++i)
    {
        audioWaveL[i] = droppedAudio.getSample(0, middle + i); 
    }
    for (int i = 0; i < audioWaveR.size(); ++i)
    {
        audioWaveR[i] = droppedAudio.getSample(1, middle + i);
    }

    // Normalize to prevent clipping
    normalizeWave(audioWaveL);
    normalizeWave(audioWaveR);
}

void DUMUMUB003AudioProcessor::fillImageWavetableFromImage()
{
    const int wavetableSize = imageWave.size();
    const int imageWidth = droppedImage.getWidth();
    const int imageHeight = droppedImage.getHeight();
    const float indent = static_cast<float>(imageWidth) / static_cast<float>(wavetableSize);

    if (wavetableSize <= 0)
    {
        return;
    }

    // Initialize wavetable
    for (int i = 0; i < wavetableSize; ++i)
    {
        imageWave[i] = 0;
    }

    /**
     * Image-to-wavetable conversion algorithm:
     * - Scan each column of pixels from left to right
     * - Find the darkest pixel in each column (lowest luminance)
     * - Map the vertical position to wavetable amplitude
     * - Create symmetric waveform by mirroring second half
     */
    for (int i = 0; i < wavetableSize; ++i)
    {
        int x = static_cast<int>(i * indent);

        if (x >= imageWidth) x = imageWidth - 1; // Clamp to image bounds

        int darkestY = imageHeight / 2; // Start from image center
        int darkestYDarkness = -1; // Track darkest pixel found
        
        for (int y = 0; y < imageHeight; ++y)
        {
            Colour pixelColour = Colours::white;
            try {
                pixelColour = droppedImage.getPixelAt(x, y);
            }
            catch (const std::exception& e)
            {
                continue; // Skip this pixel if there's an error
            }
            
            int alpha = pixelColour.getAlpha();
            if (alpha == 0) continue; // Skip fully transparent pixels

            int darkness = (pixelColour.getRed() + pixelColour.getGreen() + pixelColour.getBlue()) * alpha;

            if (darkness > darkestYDarkness)
            {
                darkestY = y;
                darkestYDarkness = darkness;
            }
        }
        if (i < wavetableSize / 2)
        {
            imageWave[i] = (static_cast<float>(darkestY) / static_cast<float>(imageHeight)) * 2.0f - 1.0f;
        }
        else
        {
            imageWave[i] = ((static_cast<float>(darkestY) / static_cast<float>(imageHeight)) * 2.0f - 1.0f) * -1.0f;
        }
    }

    normalizeWave(imageWave);
}

void DUMUMUB003AudioProcessor::normalizeWave(std::array<float, 1024>& waveTable)
{
    float max = 0;
    for (int i = 0; i < waveTable.size(); i++)
    {
        if (std::abs(waveTable[i]) > max)
        {
            max = std::abs(waveTable[i]);
        }
    }
    if (max == 0) return; // Avoid division by zero
    for (int i = 0; i < waveTable.size(); i++)
    {
        waveTable[i] = waveTable[i] / max;
    }
}

std::array<float, 1024> DUMUMUB003AudioProcessor::bufferToWaveTableL(AudioBuffer<float>& currentBuffer)
{
    std::array<float, 1024> waveTable;
    int numSamples = currentBuffer.getNumSamples();
    int numChannels = currentBuffer.getNumChannels();
    int numSamplesPerChannel = numSamples / numChannels;

    float stretchFactor = static_cast<float>(numSamplesPerChannel) / waveTable.size();

    for (int i = 0; i < waveTable.size(); ++i)
    {
        float sampleIndex = i * stretchFactor;
        int index1 = static_cast<int>(sampleIndex);
        int index2 = std::min(index1 + 1, numSamplesPerChannel - 1);
        float frac = sampleIndex - index1;

        waveTable[i] = (1.0f - frac) * currentBuffer.getSample(0, index1) + frac * currentBuffer.getSample(0, index2);
    }

    return waveTable;
}

std::array<float, 1024> DUMUMUB003AudioProcessor::bufferToWaveTableR(AudioBuffer<float>& currentBuffer)
{
 

    std::array<float, 1024> waveTable;
    int numSamples = currentBuffer.getNumSamples();
    int numChannels = currentBuffer.getNumChannels();
    int numSamplesPerChannel = numSamples / numChannels;

    float stretchFactor = static_cast<float>(numSamplesPerChannel) / waveTable.size();

    for (int i = 0; i < waveTable.size(); ++i)
    {
        float sampleIndex = i * stretchFactor;
        int index1 = static_cast<int>(sampleIndex);
        int index2 = std::min(index1 + 1, numSamplesPerChannel - 1);
        float frac = sampleIndex - index1;

        waveTable[i] = (1.0f - frac) * currentBuffer.getSample(1, index1) + frac * currentBuffer.getSample(1, index2);
    }

    return waveTable;
}

void DUMUMUB003AudioProcessor::setADSRParameters(const juce::ADSR::Parameters& params)
{
    adsrParams = params;

    // Update all voices' ADSR params
    for (int i = 0; i < synthesiser.getNumVoices(); ++i)
    {
        if (auto* voice = dynamic_cast<WavetableVoice*>(synthesiser.getVoice(i)))
            voice->setADSRParameters(params);
    }
}

//==============================================================================
// Plugin factory function
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DUMUMUB003AudioProcessor();
}

