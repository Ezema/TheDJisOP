/*
  ==============================================================================

    DeckGUI.h
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "AudioPlayer.h"
#include "WaveformDisplay.h"
//#include "PlaylistComponent.h"
//#include "PlaylistComponent.h"
//#include "MainComponent.h"

using namespace juce;

//==============================================================================
/*
*/
class DeckGUI    : public Component,
                   public Button::Listener, 
                   public Slider::Listener, 
                   public FileDragAndDropTarget,
                   public Timer
{
public:
    DeckGUI(AudioPlayer* player, AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse, std::vector<URL>* trackFilesUrl, std::vector<std::string>* trackTitles /*,PlaylistComponent* playlistComponent*/, int _guiIdentifier);
    ~DeckGUI();

    void paint (Graphics&) override;
    void resized() override;

     /** implement Button::Listener */
    void buttonClicked (Button *) override;

    void loadTrack(std::vector<URL> trackFilesUrl, int index);

    /** implement Slider::Listener */
    void sliderValueChanged (Slider *slider) override;

    bool isInterestedInFileDrag (const StringArray &files) override;
    void filesDropped (const StringArray &files, int x, int y) override; 

    void timerCallback() override; 

    WaveformDisplay waveformDisplay;    

    

    AudioPlayer* player;

private:

    int guiIdentifier;

    Label playbackSpeedLabel;
    Label volumeLabel;
    Label positionLabel;

    Label goBackOrJump5SecondsLabel;

    TextButton loadButton{ "LOAD" };

    ArrowButton goBack5secondsButton{ "GO BACK 5 SECONDS",0.5,Colours::red };
    ArrowButton jump5secondsButton{ "JUMP 5 SECONDS",0,Colours::red };

    TextButton playButton{"PLAY"};
    TextButton stopButton{"STOP"};
    //TextButton nextTrackButton{ "NEXT TRACK" };
    TextButton playFromBeginningButton{ "START OVER" };
    TextButton loopSongButton{ "LOOP DISABLED" };

    ImageButton tunningImageButton;
              
    bool loopEnabled;
    bool isPlaying;
    Slider volumeSlider; 
    Slider playbackSpeedSlider;
    Slider positionSlider;

    /*Slider testSliderOne;
    Slider testSliderTwo;
    Slider testSliderThree;
    Slider testSliderFour;
    Slider testSliderFive;
    Slider testSliderSix;*/

    

    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
