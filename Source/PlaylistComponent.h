/*
  ==============================================================================

    PlaylistComponent.h
    Created: 13 Feb 2022 6:57:26pm
    Author:  windows

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "AudioPlayer.h"
#include "DeckGUI.h"
#include <fstream>
#include "WaveformDisplay.h"
#include <map>
#include <vector>

using namespace juce;


//==============================================================================
/*
*/

class PlaylistComponent  : public Component, public TableListBoxModel, public Button::Listener, public FileDragAndDropTarget, public Timer, public AudioSource, public TextEditor::Listener
{
public:
    PlaylistComponent(AudioPlayer* player1, AudioPlayer* player2, DeckGUI* deckGUI1, DeckGUI* deckGUI2, AudioFormatManager & _formatManagerToUse, AudioThumbnailCache & cacheToUse);
    ~PlaylistComponent();

    void paint (Graphics&) override;
    void resized() override;

    void buttonClicked(Button*) override;

    void textEditorTextChanged(TextEditor&) override;

    void timerCallback() override;

    WaveformDisplay waveformDisplayRightDeck;
    WaveformDisplay waveformDisplayLeftDeck;

    int getNumRows() override;
    void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component * refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component * existingComponentToUpdate) override;    
    //Component* refreshComponentForRow(int rowNumber, bool isRowSelected, Component* existingComponentToUpdate) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    std::vector<URL> trackFilesUrl;
    std::vector<std::string> trackTitles;
    std::vector<std::string> userFilteredTrackTitles;
    std::vector<std::string> trackDuration;

    std::vector< Component*> componentsToUpdate;

    std::map<std::string, std::string> trackTitlesToDuration;
    std::map<std::string, juce::URL> trackTitlesToURLs;

    AudioPlayer* player1;
    AudioPlayer* player2;
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    std::string leftDeckNowPlaying;
    std::string rightDeckNowPlaying;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void releaseResources();
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);

    void handlePlayNextTrack(std::string deck);

private:

    TableListBox tableComponent;
    /*std::vector<std::string>* trackTitles;
    std::vector<juce::URL>* trackFilesUrl;*/    
    TextButton addSongToMyLibraryButton;    
    TextButton removeSongFromMyLibraryButton;


    AlertWindow songAlreadyAddedAlertWindow{ "Error", "You can not add a song with the same name as a song that is already in your playlist", MessageBoxIconType::InfoIcon, nullptr };

    juce::var parsedJsonDatabase;

    AudioFormatManager formatManager;
    AudioTransportSource transportSource;
    std::unique_ptr<AudioFormatReaderSource> readerSource;

    TextEditor searchBox;

    Label nowPlayingLeftDeckLabel;
    Label trackCurrentTimeLeftDeckLabel;
    Label nowPlayingRightDeckLabel;
    Label trackCurrentTimeRighttDeckLabel;

    File database;

    int oddButtonIdCounter = 1;
    int evenButtonIdCounter = 2;

    void saveToJsonDatabase();
    void readFromJsonDatabase();    

    double playlistWidth;
   
    /*std::fstream database("database.json");*/

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
