/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "AudioPlayer.h"
#include "DeckGUI.h"
#include "PlaylistComponent.h"

using namespace juce;


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainComponent   : public AudioAppComponent
{
public:
    //==============================================================================
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
    //==============================================================================
    // Your private member variables go here...
    
    std::vector<std::string> trackTitles;
    std::vector<URL> trackFilesUrl;

    AudioFormatManager formatManager;
    AudioThumbnailCache thumbCache{100}; 

    
    //PlaylistComponent playlistComponentTwo{ &player1,&player2,&deckGUI1 ,&deckGUI2, formatManager, thumbCache };
    PlaylistComponent playlistComponent{ &player1,&player2,&deckGUI1 ,&deckGUI2, formatManager, thumbCache };

    PlaylistComponent* myPointer;

    AudioPlayer player1{formatManager, &trackFilesUrl, &trackTitles };
    DeckGUI deckGUI1{&player1, formatManager, thumbCache, &trackFilesUrl, &trackTitles/*, myPointer*/};

    AudioPlayer player2{formatManager ,&trackFilesUrl, &trackTitles };
    DeckGUI deckGUI2{ &player2, formatManager, thumbCache, &trackFilesUrl, &trackTitles/*, myPointer*/};

    MixerAudioSource mixerSource; 

    
    
    

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
