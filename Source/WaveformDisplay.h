#pragma once

#include "JuceHeader.h"

using namespace juce;

class WaveformDisplay : public Component, public ChangeListener
{
public:
    // Constructor and destructor
    WaveformDisplay( AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse);
    ~WaveformDisplay();    

    // load song URL into AudioThumbnail instance
    void loadURL(URL audioURL);

    // set the relative position of the playhead
    void setPositionRelative(double pos);

private:

    // inherited from JUCE Component class
    void paint(Graphics&) override;
    void resized() override;
    // inherited from JUCE ChangeListener class
    void changeListenerCallback(ChangeBroadcaster* source) override;

    AudioThumbnail audioThumb;
    bool fileLoaded; 
    double position;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
