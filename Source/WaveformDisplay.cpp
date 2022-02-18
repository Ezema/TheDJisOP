/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 
    Author:  

  ==============================================================================
*/

#include "JuceHeader.h"
#include "WaveformDisplay.h"

using namespace juce;

//==============================================================================
WaveformDisplay::WaveformDisplay(AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse /*, int deckUID*/) : audioThumb(1000, formatManagerToUse, cacheToUse), fileLoaded(false), position(0)                          
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

  audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component    
    g.setColour (Colours::greenyellow);
    if(fileLoaded)
    {
      audioThumb.drawChannel(g, 
        getLocalBounds(), 
        0, 
        audioThumb.getTotalLength(), 
        0, 
        1.0f
      );
      g.setOpacity(1);
      g.setColour(Colours::red);
      g.fillRect(position * getWidth(), 0, 2, getHeight());            
      g.setColour(Colours::darkred);      
      g.setOpacity(0.5f);
      g.fillRect(0, 0, position * getWidth(), getHeight());      
      g.setOpacity(1);      
    }
    else 
    {
      g.setFont (16.0f);
      g.setColour(Colours::yellow);
      g.drawText ("Load a song from the playlist...", getLocalBounds(),
                  Justification::centred, true);   // draw some placeholder text

    }
}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(URL audioURL)
{
  audioThumb.clear();
  DBG("inside thumb");
  fileLoaded  = audioThumb.setSource(new URLInputSource(audioURL));
  if (fileLoaded)
  {
    DBG("file loaded");
    std::cout << "wfd: loaded! " << std::endl;
    repaint();
  }
  else {
    DBG("file not loaded");
    std::cout << "wfd: not loaded! " << std::endl;
  }

}

void WaveformDisplay::changeListenerCallback (ChangeBroadcaster *source)
{
    std::cout << "wfd: change received! " << std::endl;
    DBG("waveform callback");
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




