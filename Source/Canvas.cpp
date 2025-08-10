/*
  ==============================================================================

    Canvas.cpp
    Created: 23 Feb 2025 3:43:39pm
    Author:  Hugh Buntine

  ==============================================================================
*/

#include <JuceHeader.h>
#include "Canvas.h"
#include "PluginEditor.h"
#include "PluginProcessor.h"

//==============================================================================
Canvas::Canvas(DUMUMUB003AudioProcessor& p) : audioProcessor(p)
{
    left = p.getLeftChannelState();  // Restore saved channel states
    right = p.getRightChannelState();

    prevX = 0;
    prevY = 0;

    background = ImageFileFormat::loadFrom(BinaryData::CANVAS_png, BinaryData::CANVAS_pngSize);

    startTimerHz(30);

}

Canvas::~Canvas()
{
}

void Canvas::paint (juce::Graphics& g)
{
    DUMUMUB003AudioProcessorEditor* parentComponent = dynamic_cast<DUMUMUB003AudioProcessorEditor*>(getParentComponent());
    if (parentComponent != nullptr)
    {
      g.drawImage(background, 0, 0, getWidth(), getHeight(), 0, 0, background.getWidth(), background.getHeight());

      // Real-time oscilloscope display
        std::array<float, 1024> bufferLeft = audioProcessor.getCurrentBufferL();
        std::array<float, 1024> bufferRight = audioProcessor.getCurrentBufferR();
        drawOscilloscope(g, bufferLeft, parentComponent->getGreen(), 0);
        drawOscilloscope(g, bufferRight, parentComponent->getGreen(), 250);
      
      int lineThickness = 2;

      // Left channel wavetable visualization
      std::array<float, 1024> arrayL = parentComponent->getWaveTableL();
      g.setColour(parentComponent->getBlue());
      for (int i = 0; i < arrayL.size(); ++i)
        {
          int shift = 1; // Slight offset to prevent visual overlap
          int x = i;
          int y = static_cast<int>(((arrayL[i] + 1) / 2) * 500);
          int prevx = std::max(i - 1, 0);
          int prvy = static_cast<int>(((arrayL[prevx] + 1) / 2) * 500);
          
          g.drawLine(x + shift, y + shift, prevx + shift, prvy + shift, lineThickness); 
        }

        // Right channel wavetable visualization
        std::array<float, 1024> arrayR = parentComponent->getWaveTableR();
        g.setColour(parentComponent->getRed());
        for (int i = 0; i < arrayR.size(); ++i)
          {
            int x = i;
            int y = static_cast<int>(((arrayR[i] + 1) / 2) * 500);
            int prevx = std::max(i - 1, 0);
            int prvy = static_cast<int>(((arrayR[prevx] + 1) / 2) * 500);
            
            g.drawLine(x, y, prevx, prvy, lineThickness); 
          }
        
    }
    
}

void Canvas::resized()
{
    // No child components to layout
}

void Canvas::mouseDown(const MouseEvent& event)
{
    Point<int> mousePos = event.getPosition(); 
    int x = mousePos.getX();
    int y = mousePos.getY();

    bool inbounds = x >= 0 && x < 1024 && y >= 0 && y < 500;

    DUMUMUB003AudioProcessorEditor* parentComponent = dynamic_cast<DUMUMUB003AudioProcessorEditor*>(getParentComponent());
    if (parentComponent != nullptr && inbounds)
    {
      int index = x;
      // Convert pixel Y position to wavetable amplitude (-1.0 to 1.0)
      float value = ((float)y / 500.0f) * 2.0f - 1.0f;

      if (left){
        parentComponent->setWaveTableL(index, value);
      }
      if (right){
        parentComponent->setWaveTableR(index, value);
      }
      
      // mark the point that was just clicked
      prevX = x;
      prevY = y;

      repaint();
    }

    
}

void Canvas::mouseDrag(const MouseEvent& event)
{
    Point<int> mousePos = event.getPosition(); 
    int x = mousePos.getX();
    int y = mousePos.getY();

    bool inbounds = x >= 0 && x < 1024 && y >= 0 && y < 500;

    // Access the parent component and fill the wave table in the processor from there
    DUMUMUB003AudioProcessorEditor* parentComponent = dynamic_cast<DUMUMUB003AudioProcessorEditor*>(getParentComponent());
    if (parentComponent != nullptr && inbounds)
    {
      int index = x;
      float value = ((float)y / 500.0f) * 2.0f - 1.0f;

      if (left){
        parentComponent->setWaveTableL(index, value);
      }
      if (right){
        parentComponent->setWaveTableR(index, value);
      }

      // if the points arent side by side draw a straight line
      if (std::abs(x - prevX) > 1) {
        int x1 = std::min(x, prevX);
        int x2 = std::max(x, prevX);

        float y1, y2;
        if (x1 == x) {
          y1 = ((float)y / 500.0f) * 2.0f - 1.0f;
          y2 = ((float)prevY / 500.0f) * 2.0f - 1.0f;
        } else {
          y1 = ((float)prevY / 500.0f) * 2.0f - 1.0f;
          y2 = ((float)y / 500.0f) * 2.0f - 1.0f;
        }

        for (int i = x1; i <= x2; i++) {
          float y = y1 + (y2 - y1) * (i - x1) / (x2 - x1);

          if (left){
            parentComponent->setWaveTableL(i, y);
          }
          if (right){
            parentComponent->setWaveTableR(i, y);
          }
        }
      }

      // mark the point that was just clicked
      prevX = x;
      prevY = y;

      repaint();
    }
}

void Canvas::drawOscilloscope(Graphics& g, std::array<float, 1024> array, Colour colour, int shift)
{
  g.setColour(colour);
  for (int i = 0; i < array.size(); ++i)
    {
      int x = i;
      int y = static_cast<int>(((array[i] + 1) / 2) * 250);
      int prevx = std::max(i - 1, 0);
      int prvy = static_cast<int>(((array[prevx] + 1) / 2) * 250);
      
      g.drawLine(x, y + shift, prevx, prvy + shift, 2); 
    }
}

void Canvas::timerCallback()
{
    repaint();
}
