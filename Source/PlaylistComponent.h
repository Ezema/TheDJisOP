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
using namespace juce;


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component, public TableListBoxModel, public Button::Listener, public FileDragAndDropTarget, public Timer, public AudioSource
{
public:
    PlaylistComponent(AudioPlayer* player1, AudioPlayer* player2, DeckGUI* deckGUI1, DeckGUI* deckGUI2, AudioFormatManager & _formatManagerToUse, AudioThumbnailCache & cacheToUse);
    ~PlaylistComponent();

    void paint (Graphics&) override;
    void resized() override;

    //void buttonClicked(Button* button) override;
    void buttonClicked(Button*) override;

    void timerCallback() override;

    WaveformDisplay waveformDisplayRightDeck;
    WaveformDisplay waveformDisplayLeftDeck;

    int getNumRows() override;
    void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;

    Component * refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component * existingComponentToUpdate) override;    

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    std::vector<URL> trackFilesUrl;
    std::vector<std::string> trackTitles;
    std::vector<std::string> trackDuration;

    AudioPlayer* player1;
    AudioPlayer* player2;
    DeckGUI* deckGUI1;
    DeckGUI* deckGUI2;

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate);
    void releaseResources();
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill);

private:

    TableListBox tableComponent;
    /*std::vector<std::string>* trackTitles;
    std::vector<juce::URL>* trackFilesUrl;*/    
    TextButton addSongToMyLibraryButton;    

    AudioFormatManager formatManager;
    AudioTransportSource transportSource;
    std::unique_ptr<AudioFormatReaderSource> readerSource;

    TextEditor searchBox;

    File database;

    int oddButtonIdCounter = 1;
    int evenButtonIdCounter = 2;

    void saveToJsonDatabase();
    void readFromJsonDatabase();    
   
    /*std::fstream database("database.json");*/

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};