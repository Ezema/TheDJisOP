#pragma once

#include <JuceHeader.h>
#include "AudioPlayer.h"
#include "DeckGUI.h"
#include <fstream>
#include "WaveformDisplay.h"
#include <map>
#include <vector>

using namespace juce;

class PlaylistComponent  : public Component, public TableListBoxModel, public Button::Listener, public FileDragAndDropTarget, public Timer, public TextEditor::Listener
{
public:
    // constructor and destructor
    PlaylistComponent(AudioPlayer* player1, AudioPlayer* player2, AudioFormatManager & _formatManagerToUse, AudioThumbnailCache & cacheToUse);
    ~PlaylistComponent();

private:

    void paint(Graphics&) override;
    void paintRowBackground(Graphics&, int rowNumber, int width, int height, bool rowIsSelected) override;
    void paintCell(Graphics&, int rowNumber, int columnId, int width, int height, bool rowIsSelected) override;
    void resized() override;
    void buttonClicked(Button*) override;
    void textEditorTextChanged(TextEditor&) override;
    void timerCallback() override;
    int getNumRows() override;

    WaveformDisplay waveformDisplayRightDeck;
    WaveformDisplay waveformDisplayLeftDeck;        

    Component* refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) override;

    bool isInterestedInFileDrag(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

    TableListBox tableComponent;
    TextButton addSongToMyLibraryButton;    
    TextButton removeSongFromMyLibraryButton;

    AlertWindow songAlreadyAddedAlertWindow{ "Error", "You can not add a song with the same name of a song that is already in your playlist", MessageBoxIconType::InfoIcon, nullptr };
    AlertWindow moreThanOnceSongDragAndDroppedAlertWindow{ "Operation not permitted", "You can not drag and drop more than one song at a time", MessageBoxIconType::InfoIcon, nullptr };

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

    void persistentSaveToJsonDatabase(File * fileToSave);
    void readFromJsonDatabase();    

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

    double playlistWidth;   

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
