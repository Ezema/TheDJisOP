#pragma once

#include "JuceHeader.h"
#include "AudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

using namespace juce;

class MainComponent   : public AudioAppComponent
{
public:

    MainComponent();
    ~MainComponent();

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    
    std::vector<std::string> trackTitles;
    std::vector<URL> trackFilesUrl;

    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100}; 
       
    PlaylistComponent playlistComponent{ &player1,&player2, formatManager, thumbCache };    
    AudioPlayer player1{formatManager, &trackFilesUrl, &trackTitles };    
    AudioPlayer player2{formatManager ,&trackFilesUrl, &trackTitles };
    DeckGUI deckGUI1{ &player1, formatManager, thumbCache, &trackFilesUrl, &trackTitles, 1 };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache, &trackFilesUrl, &trackTitles, 2};

    MixerAudioSource mixerSource; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
