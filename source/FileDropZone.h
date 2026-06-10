/*
  ==============================================================================

    FileDropZone.h
    Created: 28 Feb 2025 4:47:31pm
    Author:  Hugh Buntine

    Drag-and-drop file zone for loading audio and image files.
    Provides visual feedback during drag operations and handles file processing.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

class DUMUMUB003AudioProcessorEditor;

//==============================================================================
// File drop zone component with visual feedback for drag-and-drop operations
class FileDropZone  :   public juce::Component,
                        public juce::FileDragAndDropTarget
{
public:
    FileDropZone(DUMUMUB003AudioProcessor& p) : audioProcessor(p)
    {
        // Initialize hover state and load visual assets
        isHovering = false;

        normal = ImageFileFormat::loadFrom(BinaryData::DROP_FILE_LOCATION_png, BinaryData::DROP_FILE_LOCATION_pngSize);
        hover = ImageFileFormat::loadFrom(BinaryData::DROP_FILE_LOCATION_HOVER_png, BinaryData::DROP_FILE_LOCATION_HOVER_pngSize);
    }

    ~FileDropZone() override
    {
    }

    void paint (juce::Graphics& g) override
    {
        // Render appropriate visual state based on hover status
        if (isHovering)
        {
            g.drawImage(hover, 0, 0, getWidth(), getHeight(), 0, 0, hover.getWidth(), hover.getHeight());
        }
        else
        {
            g.drawImage(normal, 0, 0, getWidth(), getHeight(), 0, 0, normal.getWidth(), normal.getHeight());
        }
    }

    void resized() override
    {
        // No child components to layout
    }

    // FileDragAndDropTarget interface implementation
    bool isInterestedInFileDrag(const StringArray& files) override
    {
        for (auto& file : files)
        {
            if (file.endsWith(".png") || file.endsWith(".jpg") || file.endsWith(".jpeg") ||
                file.endsWith(".wav") || file.endsWith(".mp3") || file.endsWith(".aiff") || file.endsWith(".flac"))
            {
                return true;
            }
        }
        return false;
    }

    void fileDragEnter(const StringArray& files, int, int) override
    {
        // Enable hover visual state when file enters drop zone
        isHovering = true;
        repaint();
    }

    void fileDragExit(const StringArray& files) override
    {
        // Disable hover visual state when file leaves drop zone
        isHovering = false;
        repaint();
    }

    void fileDragMove(const StringArray& files, int, int) override
    {
        // Track file movement within drop zone
    }

    void filesDropped(const StringArray& files, int, int) override
    {
        // Process dropped files based on type
        String file = files[0];

        // Process image files
        if (file.endsWith(".png") || file.endsWith(".jpg") || file.endsWith(".jpeg"))
        {
            audioProcessor.setImagePath(file);
            audioProcessor.setImageFromPath();

            // Update editor display
            DUMUMUB003AudioProcessorEditor* parentComponent = dynamic_cast<DUMUMUB003AudioProcessorEditor*>(getParentComponent());
            if (parentComponent != nullptr)
                {
                    parentComponent->setImageFileName(getFileName(file));
                }
        }
        // Process audio files
        else if (file.endsWith(".wav") || file.endsWith(".mp3") || file.endsWith(".aiff") || file.endsWith(".flac"))
        {
            audioProcessor.setAudioPath(file);
            audioProcessor.setAudioFromPath();

            // Update editor display
            DUMUMUB003AudioProcessorEditor* parentComponent = dynamic_cast<DUMUMUB003AudioProcessorEditor*>(getParentComponent());
            if (parentComponent != nullptr)
                {
                    parentComponent->setAudioFileName(getFileName(file));
                }
        }

        isHovering = false;
    }

#include <juce_core/juce_core.h>

// Utility function to extract filename without extension and convert to lowercase
juce::String getFileName(const juce::String& filePath) {
    // Find the last occurrence of the directory separator
    int lastSlashIndex = filePath.lastIndexOfAnyOf("/\\");
    
    // Extract the substring from the character after the last separator to the end of the string
    juce::String fileName = filePath.substring(lastSlashIndex + 1);
    
    // Find the last occurrence of the dot
    int lastDotIndex = fileName.lastIndexOf(".");
    
    // If a dot is found, remove the extension
    if (lastDotIndex != -1) {
        fileName = fileName.substring(0, lastDotIndex);
    }
    
    // Convert the file name to all lowercase
    fileName = fileName.toLowerCase();
    
    return fileName;
}

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileDropZone)

    // Visual assets for normal and hover states
    Image normal;
    Image hover;

    // Current hover state
    bool isHovering;

    DUMUMUB003AudioProcessor& audioProcessor;
};