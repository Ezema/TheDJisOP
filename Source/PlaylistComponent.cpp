/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 13 Feb 2022 6:57:26pm
    Author:  windows

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <fstream>
#include <cmath>

//==============================================================================

PlaylistComponent::PlaylistComponent(AudioPlayer* _player1, AudioPlayer* _player2, DeckGUI* deckGUI1, DeckGUI* deckGUI2, AudioFormatManager& _formatManagerToUse, AudioThumbnailCache& cacheToUse) : player1(_player1), player2(_player2),waveformDisplayLeftDeck(_formatManagerToUse, cacheToUse), waveformDisplayRightDeck(_formatManagerToUse, cacheToUse) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    /*trackFilesUrl = trackFilesUrl;
    trackTitles = trackTitles;
    player1 = player1;
    player2 = player2;
    deckGUI1 =  deckGUI1;
    deckGUI2 = deckGUI2;*/

    //formatManagerToUse = _formatManagerToUse;

    if (File::getCurrentWorkingDirectory().getChildFile("database.json").existsAsFile()) {
        database = File::getCurrentWorkingDirectory().getChildFile("database.json");
    }
    else {
        database = File::getCurrentWorkingDirectory().getChildFile("database.json");
    }

    formatManager.registerBasicFormats();

    addAndMakeVisible(waveformDisplayLeftDeck);
    addAndMakeVisible(waveformDisplayRightDeck);

    addAndMakeVisible(searchBox);

    tableComponent.getHeader().addColumn("Track title",1,1280/4);
    tableComponent.getHeader().addColumn("Duration", 2, 1280 / 4);
    tableComponent.getHeader().addColumn("", 3, 1280 / 4);
    tableComponent.getHeader().addColumn("", 4, 1280 / 4);
    tableComponent.setModel(this);

    addSongToMyLibraryButton.setButtonText("ADD NEW FILE TO MY LIBRARY");
    //addSongToMyLibraryButton.addListener();

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(addSongToMyLibraryButton);    

    addSongToMyLibraryButton.addListener(this);

    startTimer(500);
}

PlaylistComponent::~PlaylistComponent()
{
    stopTimer();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    float heightTenth = getHeight()/10;    
    float widthTenth = getWidth()/10;

    tableComponent.setBounds(0, 0, getWidth(), heightTenth*6);
    DBG(std::to_string(heightTenth * 6));
    addSongToMyLibraryButton.setBounds(0, heightTenth * 6, getWidth(), 30);
    
    waveformDisplayLeftDeck.setBounds(0, (heightTenth * 6) + 30, getWidth()/2, (heightTenth*4)-30);
    waveformDisplayRightDeck.setBounds(getWidth() / 2, (heightTenth * 6) + 30, getWidth()/2, (heightTenth*4)-30);
}


int PlaylistComponent::getNumRows(){
    //return 0;
    return trackTitles.size();
};
void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected){
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    }
    else {
        g.fillAll(Colours::darkgrey);
    }
};

void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    if (columnId == 1) {
        g.drawText(trackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, false);
    };

    if (columnId == 2) {
        g.drawText(trackDuration[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, false);
    };
    
};

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
    if (columnId == 1) {
        //existingComponentToUpdate.
    };

    if (columnId == 2) {
        //
    };
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {            
            TextButton* button = new TextButton{ "Play on deck 1" };
            button->addListener(this);                                   
            //String id{std::to_string(rowNumber) + std::to_string(columnId) };            
            String id(std::to_string(oddButtonIdCounter));
            button->setComponentID(id);
            existingComponentToUpdate = button;
            oddButtonIdCounter= oddButtonIdCounter + 2;
        }
    }    
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            TextButton* button = new TextButton{ "Play on deck 2" };
            button->addListener(this);
            //String id{ std::to_string(rowNumber) + std::to_string(columnId) };            
            String id{ std::to_string(evenButtonIdCounter)};
            button->setComponentID(id);
            existingComponentToUpdate = button;
            evenButtonIdCounter = evenButtonIdCounter+2;
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(Button* button) {    
    if (button == &addSongToMyLibraryButton)
    {
        FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {         
            auto* readFile = formatManager.createReaderFor(URL{ chooser.getResult() }.createInputStream(false));

            if (readFile != nullptr){
                std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(readFile,
                    true));
                transportSource.setSource(newSource.get(), 0, nullptr, readFile->sampleRate);
                readerSource.reset(newSource.release());
            };


            auto minutes = (int)floor(transportSource.getLengthInSeconds()/60);
            auto seconds = (int)floor(std::fmod(transportSource.getLengthInSeconds(),60));
            auto minutesAndSecondsString = std::to_string(minutes) + ":" + std::to_string(seconds);

            auto songName = chooser.getResult().getFileName().toStdString();
            trackTitles.push_back(chooser.getResult().getFileName().toStdString());
            trackDuration.push_back(minutesAndSecondsString);
            trackFilesUrl.push_back(URL{ chooser.getResult() });
            

            auto dinamicObject = std::make_unique<DynamicObject>();
            dinamicObject->setProperty("name", chooser.getResult().getFileName());
            String mio = minutesAndSecondsString;
            dinamicObject->setProperty("duration",mio);

            auto jsonObject = juce::var(dinamicObject.release());

            FileOutputStream output(database);
            JSON::writeToStream(output, jsonObject);

            tableComponent.updateContent();

            DBG(typeid(formatManager.createReaderFor(chooser.getResult())->metadataValues.getAllKeys()).name());

            DBG(typeid(chooser.getResult()).name());

            /*for (String key : formatManager.createReaderFor(chooser.getResult())->metadataValues.getAllKeys()) {
                DBG("Key: " + key + " value: " + formatManager.createReaderFor(chooser.getResult())->metadataValues.getValue(key, "unknown"));
            };*/

            /*ScopedPointer<AudioFormatReader> reader = formatManager.createReaderFor(chooser.getResult());
            if (reader) {
                for (String key : reader->metadataValues.getAllKeys()) {
                    DBG("Key: " + key + " value: " + reader->metadataValues.getValue(key, "unknown"));
                }
            }*/

            
        }        

        DBG("Directory: "+File::getCurrentWorkingDirectory().getFullPathName().toStdString());
        auto saved = JSON::parse(database);
        DBG("result: " + JSON::toString(saved));

    }
    else {
        /*FileChooser chooser{ "Select a file..." };
        if (chooser.browseForFileToOpen())
        {
            player1->loadURL(URL{ chooser.getResult() });
            waveformDisplay.loadURL(URL{ chooser.getResult() });

        }*/

        if (std::stoi(button->getComponentID().toStdString()) % 2 != 0) {
        
            float index = floor(std::stoi(button->getComponentID().toStdString()) / 2);            

            URL url = trackFilesUrl[index];
            player1->loadURL(url);
            waveformDisplayLeftDeck.loadURL(url);

            //deckGUI1->loadTrack(trackFilesUrl,(int)index);
            //DBG(std::to_string(floor(std::stoi(button->getComponentID().toStdString()) / 2)));
        }
        else {
            float index = floor(std::stoi(button->getComponentID().toStdString()) / 2) - 1;
            //DBG(trackTitles[(int)index]);
              
            URL url = trackFilesUrl[index];
            player2->loadURL(url);
            waveformDisplayRightDeck.loadURL(url);

            //deckGUI2->loadTrack(trackFilesUrl,(int)index);
            //trackFiles[(int)index].;
            
            //DBG(std::to_string(floor(std::stoi(button->getComponentID().toStdString()) / 2)-1));
        }
        //int id = std::stoi(button->getComponentID().toStdString());        
    }
    
}

bool PlaylistComponent::isInterestedInFileDrag(const StringArray& files) {
    return true;
};
void PlaylistComponent::filesDropped(const StringArray& files, int x, int y) {
    /*if (files.size() == 1)
    {
        player1->loadURL(URL{ File{files[0]} });
    }*/
};

void PlaylistComponent::timerCallback()
{
    //std::cout << "DeckGUI::timerCallback" << std::endl;
    waveformDisplayLeftDeck.setPositionRelative(
        player1->getPositionRelative());
    waveformDisplayRightDeck.setPositionRelative(
        player2->getPositionRelative());
}

void PlaylistComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
};
void PlaylistComponent::releaseResources() {
};
void PlaylistComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
};