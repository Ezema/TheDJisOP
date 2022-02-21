#pragma once

#include "JuceHeader.h"
#include "AudioPlayer.h"
#include "WaveformDisplay.h"

using namespace juce;

class DeckGUI : public Component, public Button::Listener, public Slider::Listener, public Timer
{
public:
    DeckGUI(AudioPlayer* player, AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse, std::vector<URL>* trackFilesUrl, std::vector<std::string>* trackTitles, int _guiIdentifier);
    ~DeckGUI();
    
private:    
    void buttonClicked(Button*) override;
    void sliderValueChanged(Slider* slider) override;
    void timerCallback() override;
    void paint(Graphics&) override;
    void resized() override;
    
    AudioPlayer* player;

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

    bool loopEnabled;
    bool isPlaying;
    Slider volumeSlider; 
    Slider playbackSpeedSlider;
    Slider positionSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
