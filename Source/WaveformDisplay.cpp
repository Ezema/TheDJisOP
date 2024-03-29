#include "JuceHeader.h"
#include "WaveformDisplay.h"

using namespace juce;

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse) : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)                          
{
  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);
    g.setColour (Colours::greenyellow);
    
    if(fileLoaded){
      audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0f);
      g.setOpacity(1);
      g.setColour(Colours::red);
      g.fillRect(position * getWidth(), 0, 2, getHeight());            
      g.setColour(Colours::darkred);      
      g.setOpacity(0.5f);
      g.fillRect(0, 0, position * getWidth(), getHeight());      
      g.setOpacity(1);      
    }
    else {
      g.setFont (16.0f);
      g.setColour(Colours::yellow);
      g.drawText ("Load a song from the playlist...", getLocalBounds(), Justification::centred, true);  
    }
}

void WaveformDisplay::resized()
{    
}

void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();  
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
  if (fileLoaded)
  {
    repaint();
  }
  else {
  }
}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{    
    repaint();
}

void WaveformDisplay::setPositionRelative(double pos)
{
  if (pos != position)
  {
    position = pos;
    repaint();
  }
}




