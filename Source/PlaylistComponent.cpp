/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 13 Feb 2022 6:57:26pm
    Author:  windows

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PlaylistComponent.h"
#include <fstream>
#include <cmath>


//CustomVisualizer::CustomVisualizer() {
//    holdThisOne = true;
//};

//==============================================================================

PlaylistComponent::PlaylistComponent(AudioPlayer* _player1, AudioPlayer* _player2, DeckGUI* deckGUI1, DeckGUI* deckGUI2, AudioFormatManager& _formatManagerToUse, AudioThumbnailCache& cacheToUse) : player1(_player1), player2(_player2),waveformDisplayLeftDeck(_formatManagerToUse, cacheToUse/*, 1*/), waveformDisplayRightDeck(_formatManagerToUse, cacheToUse/*, 2*/) {
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
        

    if (File::getCurrentWorkingDirectory().getChildFile("database.json").existsAsFile()) {
        database = File::getCurrentWorkingDirectory().getChildFile("database.json");
        
        parsedJsonDatabase = juce::JSON::parse(database);
        if (parsedJsonDatabase.isArray()) {
            for (auto item : *parsedJsonDatabase.getArray()) {
                if (auto object = item.getDynamicObject()) {                    
                    std::string parsedTrackTitle = object->getProperties().getValueAt(0).toString().toStdString();
                    trackTitles.push_back(parsedTrackTitle);
                    userFilteredTrackTitles.push_back(parsedTrackTitle);
                    trackTitlesToDuration[parsedTrackTitle] = object->getProperties().getValueAt(1).toString().toStdString();
                    trackTitlesToURLs[parsedTrackTitle] = URL{ object->getProperties().getValueAt(2).toString() };

                }
            }
        }
    }
    else {
        database = File::getCurrentWorkingDirectory().getChildFile("database.json");                
        parsedJsonDatabase = juce::JSON::parse(database);

    }

    formatManager.registerBasicFormats();

    addAndMakeVisible(waveformDisplayLeftDeck);
    addAndMakeVisible(waveformDisplayRightDeck);
    

    playlistWidth = 1280;
    
    addAndMakeVisible(searchBox);
    addAndMakeVisible(nowPlayingLeftDeckLabel);
    addAndMakeVisible(nowPlayingRightDeckLabel);
    addAndMakeVisible(trackCurrentTimeLeftDeckLabel);
    addAndMakeVisible(trackCurrentTimeRighttDeckLabel);    

    tableComponent.getHeader().addColumn("Track title",1, playlistWidth / 2 / 2);
    tableComponent.getHeader().addColumn("Duration", 2, playlistWidth /2/2);
    tableComponent.getHeader().addColumn("Play on left deck", 3, playlistWidth / 2 / 3);
    tableComponent.getHeader().addColumn("Play on right deck", 4, playlistWidth / 2 / 3);
    tableComponent.getHeader().addColumn("Remove", 5, playlistWidth / 2 / 3);
    tableComponent.setModel(this);    

    nowPlayingRightDeckLabel.setFont(juce::Font(18.0f, juce::Font::bold));
    nowPlayingRightDeckLabel.setText("Now playing: " + rightDeckNowPlaying, juce::dontSendNotification);
    nowPlayingRightDeckLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    nowPlayingRightDeckLabel.setColour(juce::Label::backgroundColourId, Colours::black);
    nowPlayingRightDeckLabel.setJustificationType(juce::Justification::topLeft);
    
    nowPlayingLeftDeckLabel.setColour(juce::Label::textColourId, juce::Colours::green);
    nowPlayingLeftDeckLabel.setColour(juce::Label::backgroundColourId, Colours::white);
    nowPlayingLeftDeckLabel.setFont(juce::Font(18.0f, juce::Font::bold));    
    nowPlayingLeftDeckLabel.setText("Now playing: " + leftDeckNowPlaying, juce::dontSendNotification);
    nowPlayingLeftDeckLabel.setJustificationType(juce::Justification::topLeft);

    trackCurrentTimeRighttDeckLabel.setFont(Font(18.0f));
    trackCurrentTimeRighttDeckLabel.setText("", juce::dontSendNotification);
    trackCurrentTimeRighttDeckLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    trackCurrentTimeRighttDeckLabel.setColour(juce::Label::backgroundColourId, Colours::black);
    trackCurrentTimeRighttDeckLabel.setJustificationType(juce::Justification::bottomLeft);
    
    trackCurrentTimeLeftDeckLabel.setFont(Font(18.0f));
    trackCurrentTimeLeftDeckLabel.setText("", juce::dontSendNotification);
    trackCurrentTimeLeftDeckLabel.setColour(Label::textColourId, juce::Colours::green);
    trackCurrentTimeLeftDeckLabel.setColour(Label::backgroundColourId, Colours::white);
    trackCurrentTimeLeftDeckLabel.setJustificationType(juce::Justification::bottomLeft);

    addSongToMyLibraryButton.setButtonText("ADD NEW FILE TO MY LIBRARY");        
    addSongToMyLibraryButton.setColour(TextButton::buttonColourId, Colours::mediumvioletred);
    addSongToMyLibraryButton.setColour(TextButton::textColourOnId, Colours::black);
    addSongToMyLibraryButton.setColour(TextButton::textColourOffId, Colours::black);
    //addSongToMyLibraryButton.addListener();

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(addSongToMyLibraryButton);    

    addSongToMyLibraryButton.addListener(this);

    searchBox.addListener(this);
    searchBox.setTextToShowWhenEmpty(String{ "Search in your library" },Colours::black);    
    searchBox.setColour(TextEditor::textColourId, Colours::black);
    searchBox.setColour(TextEditor::backgroundColourId, Colours::lightpink);    
    searchBox.setFont(juce::Font(15.0f, juce::Font::bold));
    searchBox.setJustification(Justification::centredLeft);
    
    /*searchBox.setColour(1, juce::Colours::white);
    searchBox.setColour(2, juce::Colours::white);
    searchBox.setColour(3, juce::Colours::white);
    searchBox.setColour(4, juce::Colours::white);*/

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
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    float heightTenth = getHeight()/10;    
    float widthTenth = 1280/10;


    playlistWidth = getWidth();

    tableComponent.setBounds(0, 0, getWidth(), heightTenth*3);
    searchBox.setBounds(0, heightTenth * 3, getWidth(), 30);        
    addSongToMyLibraryButton.setBounds(0, heightTenth * 3 + 30, getWidth(), 30);
    nowPlayingLeftDeckLabel.setBounds(0,heightTenth*3+60,getWidth()/2,18);
    nowPlayingRightDeckLabel.setBounds(getWidth()/2,heightTenth*3+60,getWidth()/2,18);
    trackCurrentTimeLeftDeckLabel.setBounds(0, heightTenth *3+78, getWidth() / 2, 20);
    trackCurrentTimeRighttDeckLabel.setBounds(getWidth() / 2, heightTenth * 3+78, getWidth() / 2, 20);
    waveformDisplayLeftDeck.setBounds(0, (heightTenth * 3) + 98, getWidth()/2, (heightTenth*5)-27);
    waveformDisplayRightDeck.setBounds(getWidth() / 2, (heightTenth * 3) + 98, getWidth()/2, (heightTenth*5)-27);    

    tableComponent.autoSizeAllColumns();
}


int PlaylistComponent::getNumRows(){
    //return 0;
    return userFilteredTrackTitles.size();
    //return trackTitles.size();
};
void PlaylistComponent::paintRowBackground(Graphics& g, int rowNumber, int width, int height, bool rowIsSelected){
    if (rowIsSelected) {
        g.fillAll(Colours::orange);
    }
    else {
        g.fillAll(Colours::lightgreen);
    }
};

void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    
    if (columnId == 1) {
        g.drawText(userFilteredTrackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, false);
    };

    if (columnId == 2) {
        g.drawText(trackTitlesToDuration[userFilteredTrackTitles[rowNumber]], 2, 0, width - 4, height, Justification::centredLeft, false);
    };    
    
};


Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
    
    if (columnId == 1) {        
    };

    if (columnId == 2) {
        //
    };
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {                        
            TextButton* button = new TextButton{ "Play on white deck" };
            button->addListener(this);                              
            button->setColour(TextButton::buttonColourId, Colours::white);            
            button->setColour(TextButton::textColourOnId, Colours::black);
            button->setColour(TextButton::textColourOffId, Colours::black);
            button->setComponentID("1" + userFilteredTrackTitles[rowNumber]);
            

            existingComponentToUpdate = button;
            componentsToUpdate.push_back(button);
            //oddButtonIdCounter= oddButtonIdCounter + 2;
        }
        else {
            TextButton* button = new TextButton{ "Play on black deck" };
            button->addListener(this);
            button->setColour(TextButton::buttonColourId, Colours::white);
            button->setColour(TextButton::textColourOnId, Colours::black);
            button->setColour(TextButton::textColourOffId, Colours::black);
            button->setComponentID("1" + userFilteredTrackTitles[rowNumber]);
            existingComponentToUpdate = button;
            componentsToUpdate.push_back(button);
        }
    }    
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            
            TextButton* button = new TextButton{ "Play on white deck" };
            button->addListener(this);
            button->setColour(TextButton::buttonColourId, Colours::black);
            //String id{ std::to_string(rowNumber) + std::to_string(columnId) };            
            //String id{ std::to_string(evenButtonIdCounter)};
            //button->setComponentID(id);            
            button->setComponentID("2" + userFilteredTrackTitles[rowNumber]);           
            
            existingComponentToUpdate = button;
            componentsToUpdate.push_back(button);
            //evenButtonIdCounter = evenButtonIdCounter+2;
        }
        else {
            TextButton* button = new TextButton{ "Play on black deck" };
            button->addListener(this);
            button->setColour(TextButton::buttonColourId, Colours::black);
            button->setComponentID("2" + userFilteredTrackTitles[rowNumber]);
            existingComponentToUpdate = button;
            componentsToUpdate.push_back(button);
        }
    }
    if (columnId == 5) {
        if (existingComponentToUpdate == nullptr) {
            
            TextButton* button = new TextButton{ "Remove from my library" };
            button->addListener(this);
            button->setColour(TextButton::buttonColourId, Colours::red);
            button->setColour(TextButton::textColourOnId, Colours::black);
            button->setColour(TextButton::textColourOffId, Colours::black);
            //String id{ std::to_string(rowNumber) + std::to_string(columnId) };            
            //String id{ std::to_string(evenButtonIdCounter)};
            //button->setComponentID(id);
            
            button->setComponentID(userFilteredTrackTitles[rowNumber]);
            
            existingComponentToUpdate = button;
            componentsToUpdate.push_back(button);
            //evenButtonIdCounter = evenButtonIdCounter + 2;
        }
        else {
            TextButton* button = new TextButton{ "Remove from my library" };
            button->addListener(this);
            button->setColour(TextButton::buttonColourId, Colours::red);
            button->setColour(TextButton::textColourOnId, Colours::black);
            button->setColour(TextButton::textColourOffId, Colours::black);
            button->setComponentID(userFilteredTrackTitles[rowNumber]);
            existingComponentToUpdate = button;
            componentsToUpdate.push_back(button);
        }
    }
    return existingComponentToUpdate;
}

void PlaylistComponent::textEditorTextChanged(TextEditor&) {
        
    std::string searchTerm = searchBox.getText().toStdString();       
    
    //std::string searchTermInLowerCase = searchTerm;
    //std::transform(searchTermInLowerCase.begin(), searchTermInLowerCase.end(), searchTermInLowerCase.begin(), [](unsigned char c) { return std::tolower(c); });
    std::vector<std::string> trackTitlesInLowerCase = trackTitles;
   

    std::vector<std::string> auxiliaryCopy;

    std::copy_if(trackTitles.begin(), trackTitles.end(), std::back_inserter(auxiliaryCopy), [searchTerm](std::string song) {return song.rfind(searchTerm,0)==0; });
    //std::copy_if(trackTitles.begin(), trackTitles.end(), std::back_inserter(trackFilesUrl), [searchTerm](std::string song) {return song.rfind(searchTerm, 0) == 0; });

    
    userFilteredTrackTitles = auxiliaryCopy;

    tableComponent.updateContent();
    
};

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

            if (!(std::find(trackTitles.begin(), trackTitles.end(), chooser.getResult().getFileName().toStdString()) != trackTitles.end())) {

                auto minutes = (int)floor(transportSource.getLengthInSeconds() / 60);
                auto seconds = (int)floor(std::fmod(transportSource.getLengthInSeconds(), 60));
                auto minutesAndSecondsString = std::to_string(minutes) + ":" + std::to_string(seconds);

                auto songName = chooser.getResult().getFileName().toStdString();
                userFilteredTrackTitles.push_back(chooser.getResult().getFileName().toStdString());
                trackTitles.push_back(chooser.getResult().getFileName().toStdString());

                trackTitlesToDuration[chooser.getResult().getFileName().toStdString()] = minutesAndSecondsString;
                trackTitlesToURLs[chooser.getResult().getFileName().toStdString()] = URL{ chooser.getResult() };


                auto dinamicObject = std::make_unique<DynamicObject>();
                dinamicObject->setProperty("name", chooser.getResult().getFileName());
                String savedTrackDurationString = minutesAndSecondsString;
                dinamicObject->setProperty("duration", savedTrackDurationString);
                URL savedTrackUrl = URL{ chooser.getResult() };
                //String savedStringTrackUrl = savedTrackUrl.toString(false);
                dinamicObject->setProperty("url", savedTrackUrl.toString(false));

                auto jsonObject = juce::var(dinamicObject.release());

                parsedJsonDatabase.append(jsonObject);

                database.deleteFile();
                database = File::getCurrentWorkingDirectory().getChildFile("database.json");

                FileOutputStream outputFile(database);
                JSON::writeToStream(outputFile, parsedJsonDatabase);

                tableComponent.updateContent();
            }else{
                songAlreadyAddedAlertWindow.showMessageBox(MessageBoxIconType::InfoIcon, "Error", "You can not add a song with the same name of a song that is already in your playlist", String("OK"), nullptr);
            }
                
            
        }        
    }
    else {
        
        if (button->getComponentID().toStdString().substr(0,1).compare("1") == 0) {            
            leftDeckNowPlaying = button->getComponentID().toStdString().substr(1);
            nowPlayingLeftDeckLabel.setText("Now playing:   " + leftDeckNowPlaying, juce::dontSendNotification);
                                    
            auto minutes = (int)floor(player1->getPositionRelative() * transportSource.getLengthInSeconds() / 60);
            auto seconds = (int)floor(std::fmod(player1->getPositionRelative() * transportSource.getLengthInSeconds(), 60));
            auto minutesAndSecondsString = std::to_string(minutes) + ":" + std::to_string(seconds);
            trackCurrentTimeLeftDeckLabel.setText(minutesAndSecondsString,juce::dontSendNotification);
            
            URL url = trackTitlesToURLs[button->getComponentID().toStdString().substr(1)];
            player1->loadURL(url);
            waveformDisplayLeftDeck.loadURL(url);
        }
        else if (button->getComponentID().toStdString().substr(0,1).compare("2") == 0) {            
            rightDeckNowPlaying = button->getComponentID().toStdString().substr(1);
            nowPlayingRightDeckLabel.setText("Now playing:  " + rightDeckNowPlaying, juce::dontSendNotification);
            
            auto minutes = (int)floor(player2->getPositionRelative() * transportSource.getLengthInSeconds() / 60);
            auto seconds = (int)floor(std::fmod(player2->getPositionRelative() * transportSource.getLengthInSeconds(), 60));
            auto minutesAndSecondsString = std::to_string(minutes) + ":" + std::to_string(seconds);
            trackCurrentTimeRighttDeckLabel.setText(minutesAndSecondsString, juce::dontSendNotification);

            URL url = trackTitlesToURLs[button->getComponentID().toStdString().substr(1)];
            player2->loadURL(url);
            waveformDisplayRightDeck.loadURL(url);
        }
        else {

            

            std::vector<std::string> auxiliaryCopyOne;
            std::vector<std::string> auxiliaryCopyTwo;

            std::string songToRemove = button->getComponentID().toStdString();
            
            
            std::copy_if(trackTitles.begin(), trackTitles.end(), std::back_inserter(auxiliaryCopyOne), [songToRemove](std::string song) {return song.compare(songToRemove) != 0; });
            std::copy_if(userFilteredTrackTitles.begin(), userFilteredTrackTitles.end(), std::back_inserter(auxiliaryCopyTwo), [songToRemove](std::string song) {return song.compare(songToRemove) != 0; });
            trackTitles = auxiliaryCopyOne;
            userFilteredTrackTitles = auxiliaryCopyTwo;
            
            trackTitlesToURLs.erase(button->getComponentID().toStdString());                        
            trackTitlesToDuration.erase(button->getComponentID().toStdString());                                    
            
            

            tableComponent.updateContent();

            //remove from parsed db

            database.deleteFile();
            database = File::getCurrentWorkingDirectory().getChildFile("database.json");
            parsedJsonDatabase = JSON::parse(database);

            for (size_t i = 0; i < trackTitles.size(); i++)
            {
                auto dinamicObject = std::make_unique<DynamicObject>();
                dinamicObject->setProperty("name", String{ trackTitles[i] });
                
                String savedTrackDurationString = trackTitlesToDuration[trackTitles[i]];
                dinamicObject->setProperty("duration", savedTrackDurationString);
                
                URL savedTrackUrl = trackTitlesToURLs[trackTitles[i]];
                dinamicObject->setProperty("url", savedTrackUrl.toString(false));

                auto jsonObject = juce::var(dinamicObject.release());

                parsedJsonDatabase.append(jsonObject);
            }

            ////write in file            
            FileOutputStream outputFile(database);
            JSON::writeToStream(outputFile, parsedJsonDatabase);

        }
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
    waveformDisplayLeftDeck.setPositionRelative(player1->getPositionRelative()/100);
    waveformDisplayRightDeck.setPositionRelative(player2->getPositionRelative()/100);
        
    auto currentMinutes = (int)floor(player1->getPositionRelative()/100 * player1->getLengthInSeconds() / 60);    
    auto currentSeconds = (int)floor(std::fmod(player1->getPositionRelative()/100 * player1->getLengthInSeconds(), 60));    
    auto totalMinutesDuration = (int)floor(player1->getLengthInSeconds() / 60);
    auto totalSecondsDuration = (int)floor(std::fmod(player1->getLengthInSeconds(), 60));
    auto minutesAndSecondsString = std::to_string(currentMinutes) + ":" + std::to_string(currentSeconds) + " / " + std::to_string(totalMinutesDuration) + ":" + std::to_string(totalSecondsDuration);
    
    trackCurrentTimeLeftDeckLabel.setText(minutesAndSecondsString, juce::dontSendNotification);

    currentMinutes = (int)floor(player2->getPositionRelative() / 100 * player2->getLengthInSeconds() / 60);
    currentSeconds = (int)floor(std::fmod(player2->getPositionRelative() / 100 * player2->getLengthInSeconds(), 60));
    totalMinutesDuration = (int)floor(player2->getLengthInSeconds() / 60);
    totalSecondsDuration = (int)floor(std::fmod(player2->getLengthInSeconds(), 60));
    minutesAndSecondsString = std::to_string(currentMinutes) + ":" + std::to_string(currentSeconds) + " / " + std::to_string(totalMinutesDuration) + ":" + std::to_string(totalSecondsDuration);
    trackCurrentTimeRighttDeckLabel.setText(minutesAndSecondsString, juce::dontSendNotification);
}

void PlaylistComponent::prepareToPlay(int samplesPerBlockExpected, double sampleRate) {
  
};
void PlaylistComponent::releaseResources() {
};
void PlaylistComponent::getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) {
  
};

void PlaylistComponent::handlePlayNextTrack(std::string deck) {
    
    if (deck.compare("left") == 0) {
        for (size_t i = 0; i < userFilteredTrackTitles.size(); i++)
        {
            if (userFilteredTrackTitles[i].compare(leftDeckNowPlaying) == 0)
            {
                URL url = trackTitlesToURLs[userFilteredTrackTitles[i+1]];
                player1->loadURL(url);
                waveformDisplayLeftDeck.loadURL(url);
            }
        }
    }
    if (deck.compare("right") == 0) {
        for (size_t i = 0; i < userFilteredTrackTitles.size(); i++)
        {
            if (userFilteredTrackTitles[i].compare(rightDeckNowPlaying) == 0)
            {
                URL url = trackTitlesToURLs[userFilteredTrackTitles[i + 1]];
                player2->loadURL(url);
                waveformDisplayRightDeck.loadURL(url);
            }
        }
    }
    
};