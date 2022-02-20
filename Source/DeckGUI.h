#pragma once

#include "JuceHeader.h"
#include "AudioPlayer.h"
#include "WaveformDisplay.h"

using namespace juce;

class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget,
                   public Timer
{
public:
    DeckGUI(AudioPlayer* player, AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse, std::vector<URL>* trackFilesUrl, std::vector<std::string>* trackTitles, int _guiIdentifier);
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;     
    void buttonClicked (Button *) override;
    void sliderValueChanged(Slider* slider) override;
    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;
    void timerCallback() override;

    void loadTrack(std::vector<URL> trackFilesUrl, int index);
    WaveformDisplay waveformDisplay;    
    AudioPlayer* player;

private:

    int guiIdentifier;

    Label playbackSpeedLabel;
    Label volumeLabel;
    Label positionLabel;

    Label goBackOrGoForward5SecondsLabel;

    TextButton loadButton{ "LOAD" };

    ArrowButton goBack5secondsButton{ "GO BACK 5 SECONDS",0.5,Colours::red };
    ArrowButton goForward5SecondsButton{ "JUMP 5 SECONDS",0,Colours::red };

    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    TextButton playFromBeginningButton{ "START OVER" };
    TextButton loopSongButton{ "LOOP DISABLED" };

    ImageButton tunningImageButton;
              
    bool loopEnabled;
    bool isPlaying;
    Slider volumeSlider; 
    Slider playbackSpeedSlider;
    Slider positionSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
