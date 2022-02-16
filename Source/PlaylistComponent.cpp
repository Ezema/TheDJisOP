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

    //juce::var parsedJsonDatabase = juce::JSON::parse(database);

    //DBG("type id");
    //DBG(typeid(parsedJsonDatabase).name());
    

    //if (parsedJsonDatabase.isArray()) {
    //    for (auto item : *parsedJsonDatabase.getArray()) {
    //        if (auto object = item.getDynamicObject()) {
    //            DBG(typeid(object).name());
    //            
    //            std::string parsedTrackTitle = object->getProperties().getValueAt(0).toString().toStdString();
    //            trackTitles.push_back(parsedTrackTitle);
    //            userFilteredTrackTitles.push_back(parsedTrackTitle);
    //            trackTitlesToDuration[parsedTrackTitle] = object->getProperties().getValueAt(1).toString().toStdString();
    //            trackTitlesToURLs[parsedTrackTitle] = URL{ object->getProperties().getValueAt(2).toString() };
    //            
    //        }
    //            /*if (object->hasProperty("Valid"))
    //                object->setProperty("Valid", false);*/
    //    }
    //}

    

    formatManager.registerBasicFormats();

    addAndMakeVisible(waveformDisplayLeftDeck);
    addAndMakeVisible(waveformDisplayRightDeck);

    addAndMakeVisible(searchBox);

    tableComponent.getHeader().addColumn("Track title",1,1280/2/2);
    tableComponent.getHeader().addColumn("Duration", 2, 1280 /2/2);
    tableComponent.getHeader().addColumn("", 3, 1280 / 2 / 3);
    tableComponent.getHeader().addColumn("", 4, 1280 / 2 / 3);
    tableComponent.getHeader().addColumn("", 5, 1280 / 2 / 3);
    tableComponent.setModel(this);

    addSongToMyLibraryButton.setButtonText("ADD NEW FILE TO MY LIBRARY");
    //addSongToMyLibraryButton.addListener();

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(addSongToMyLibraryButton);    

    addSongToMyLibraryButton.addListener(this);

    searchBox.addListener(this);
    searchBox.setTextToShowWhenEmpty(String{ "Search in your library" },juce::Colours::greenyellow);    
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
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    float heightTenth = getHeight()/10;    
    float widthTenth = getWidth()/10;

    tableComponent.setBounds(0, 0, getWidth(), heightTenth*6-30);
    searchBox.setBounds(0, heightTenth * 6 - 30, getWidth(), 30);    
    addSongToMyLibraryButton.setBounds(0, heightTenth * 6, getWidth(), 30);
    
    waveformDisplayLeftDeck.setBounds(0, (heightTenth * 6) + 30, getWidth()/2, (heightTenth*4)-30);
    waveformDisplayRightDeck.setBounds(getWidth() / 2, (heightTenth * 6) + 30, getWidth()/2, (heightTenth*4)-30);
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
        g.fillAll(Colours::darkgrey);
    }
};

void PlaylistComponent::paintCell(Graphics& g, int rowNumber, int columnId, int width, int height, bool rowIsSelected) {
    if (columnId == 1) {
        /*if (userFilteredTrackTitles.size() > 0) {
            g.drawText(userFilteredTrackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, false);
        }
        else {*/
            g.drawText(userFilteredTrackTitles[rowNumber], 2, 0, width - 4, height, Justification::centredLeft, false);
        /*}        */
    };

    if (columnId == 2) {
        g.drawText(trackTitlesToDuration[userFilteredTrackTitles[rowNumber]], 2, 0, width - 4, height, Justification::centredLeft, false);
    };
    
};

//Component* PlaylistComponent::refreshComponentForRow(int rowNumber, bool isRowSelected, Component* existingComponentToUpdate) {
//    DBG("refresh plox");
//    return existingComponentToUpdate;
//};

Component* PlaylistComponent::refreshComponentForCell(int rowNumber, int columnId, bool isRowSelected, Component* existingComponentToUpdate) {
    //DBG("INSIDE REFRESH CALLED");
    if (columnId == 1) {        
    };

    if (columnId == 2) {
        //
    };
    if (columnId == 3) {
        if (existingComponentToUpdate == nullptr) {            
            /*DBG("INSIDE REFRESH 3");*/
            /*DBG("rowNumber");
            DBG(rowNumber);
            DBG("song name:");
            DBG(userFilteredTrackTitles[rowNumber]);*/
            TextButton* button = new TextButton{ "Play on deck 1" };
            button->addListener(this);                                   
            //String id{std::to_string(rowNumber) + std::to_string(columnId) };                                   
            //String id(std::to_string(oddButtonIdCounter));
            //button->setComponentID(id);
            button->setComponentID("1" + userFilteredTrackTitles[rowNumber]);
            
            /*DBG("rowNumber");
            DBG(rowNumber);
            DBG("play first deck component id");
            DBG(button->getComponentID());*/

            existingComponentToUpdate = button;
            componentsToUpdate.push_back(button);
            //oddButtonIdCounter= oddButtonIdCounter + 2;
        }
        else {
            TextButton* button = new TextButton{ "Play on deck 1" };
            button->addListener(this);
            button->setComponentID("1" + userFilteredTrackTitles[rowNumber]);
            existingComponentToUpdate = button;
            componentsToUpdate.push_back(button);
        }
    }    
    if (columnId == 4) {
        if (existingComponentToUpdate == nullptr) {
            /*DBG("INSIDE REFRESH 4");*/
            /*DBG("rowNumber");
            DBG(rowNumber);
            DBG("song name:");
            DBG(userFilteredTrackTitles[rowNumber]);*/
            TextButton* button = new TextButton{ "Play on deck 2" };
            button->addListener(this);
            //String id{ std::to_string(rowNumber) + std::to_string(columnId) };            
            //String id{ std::to_string(evenButtonIdCounter)};
            //button->setComponentID(id);            
            button->setComponentID("2" + userFilteredTrackTitles[rowNumber]);           
            
            /*DBG("rowNumber");
            DBG(rowNumber);
            DBG("play second deck component id");
            DBG(button->getComponentID());*/
            existingComponentToUpdate = button;
            componentsToUpdate.push_back(button);
            //evenButtonIdCounter = evenButtonIdCounter+2;
        }
        else {
            TextButton* button = new TextButton{ "Play on deck 2" };
            button->addListener(this);
            button->setComponentID("2" + userFilteredTrackTitles[rowNumber]);
            existingComponentToUpdate = button;
            componentsToUpdate.push_back(button);
        }
    }
    if (columnId == 5) {
        if (existingComponentToUpdate == nullptr) {
            /*DBG("INSIDE REFRESH 5");*/
            TextButton* button = new TextButton{ "Remove from my library" };
            button->addListener(this);
            //String id{ std::to_string(rowNumber) + std::to_string(columnId) };            
            //String id{ std::to_string(evenButtonIdCounter)};
            //button->setComponentID(id);
            
            button->setComponentID(userFilteredTrackTitles[rowNumber]);
            /*DBG("rowNumber");
            DBG(rowNumber);
            DBG("remove component id");
            DBG(button->getComponentID());*/
            existingComponentToUpdate = button;
            componentsToUpdate.push_back(button);
            //evenButtonIdCounter = evenButtonIdCounter + 2;
        }
        else {
            TextButton* button = new TextButton{ "Remove from my library" };
            button->addListener(this);
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
    //std::transform(trackTitlesInLowerCase.begin(), trackTitlesInLowerCase.end(), trackTitlesInLowerCase.begin(), ::tolower);
    
    /*for (std::string& song : trackTitlesInLowerCase) {
        std::transform(song.begin(), song.end(), song.begin(),
            [](char c) { return std::tolower(c); });
    }*/

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


            auto minutes = (int)floor(transportSource.getLengthInSeconds()/60);
            auto seconds = (int)floor(std::fmod(transportSource.getLengthInSeconds(),60));
            auto minutesAndSecondsString = std::to_string(minutes) + ":" + std::to_string(seconds);

            auto songName = chooser.getResult().getFileName().toStdString();
            userFilteredTrackTitles.push_back(chooser.getResult().getFileName().toStdString());
            trackTitles.push_back(chooser.getResult().getFileName().toStdString());
            //trackDuration.push_back(minutesAndSecondsString);
            //trackFilesUrl.push_back(URL{ chooser.getResult() });

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
            //DBG("Created new json");

            /*tableComponent.updateContent();
            for (size_t i = 0; i <= tableComponent.getNumRows(); i++)
            {
                tableComponent.repaintRow(i);
            }*/
            tableComponent.updateContent();

            //DBG(typeid(formatManager.createReaderFor(chooser.getResult())->metadataValues.getAllKeys()).name());

            //DBG(typeid(chooser.getResult()).name());

            /*for (String key : formatManager.createReaderFor(chooser.getResult())->metadataValues.getAllKeys()) {
                DBG("Key: " + key + " value: " + formatManager.createReaderFor(chooser.getResult())->metadataValues.getValue(key, "unknown"));
            };*/

            /*ScopedPointer<AudioFormatReader> reader = formatManager.createReaderFor(chooser.getResult());
            if (reader) {
                for (String key : reader->metadataValues.getAllKeys()) {
                    DBG("Key: " + key + " value: " + reader->metadataValues.getValue(key, "unknown"));
                }
            }*/

            /*{
                "someNestedObject": {
                    "someProperty": 25,
                        "someOtherProperty" : "a String"
                },
                    "someOtherRegularProperty" : false
            }*/


            /*juce::var parsedJson;
            if (juce::JSON::parse(json, parsedJson).wasOK()) {
                int value1 = parsedJson["someNestedObject"]["someProperty"];
                String value2 = parsedJson["someNestedObject"]["someOtherProperty"];
                bool value3 = parsedJson["someOtherRegularProperty"];
            }*/

            
        }        

        //DBG("Directory: "+File::getCurrentWorkingDirectory().getFullPathName().toStdString());
        //auto saved = JSON::parse(database);
        //DBG("result: " + JSON::toString(saved));

    }
    else {
        
        if (button->getComponentID().toStdString().substr(0,1).compare("1") == 0) {            
            URL url = trackTitlesToURLs[button->getComponentID().toStdString().substr(1)];
            player1->loadURL(url);
            waveformDisplayLeftDeck.loadURL(url);
        }
        else if (button->getComponentID().toStdString().substr(0,1).compare("2") == 0) {            
            URL url = trackTitlesToURLs[button->getComponentID().toStdString().substr(1)];
            player2->loadURL(url);
            waveformDisplayRightDeck.loadURL(url);
        }
        else {

            

            std::vector<std::string> auxiliaryCopyOne;
            std::vector<std::string> auxiliaryCopyTwo;

            std::string songToRemove = button->getComponentID().toStdString();
            
            /*DBG("Clicked remove, Song to remove");
            DBG(songToRemove);*/
            
            std::copy_if(trackTitles.begin(), trackTitles.end(), std::back_inserter(auxiliaryCopyOne), [songToRemove](std::string song) {return song.compare(songToRemove) != 0; });
            std::copy_if(userFilteredTrackTitles.begin(), userFilteredTrackTitles.end(), std::back_inserter(auxiliaryCopyTwo), [songToRemove](std::string song) {return song.compare(songToRemove) != 0; });
            trackTitles = auxiliaryCopyOne;
            userFilteredTrackTitles = auxiliaryCopyTwo;
            
            /*DBG("after deleting this is how userFilteredTrackTitles looks ");
            for (size_t i = 0; i < userFilteredTrackTitles.size(); i++)
            {
                DBG(userFilteredTrackTitles[i]);
            }

            DBG("after deleting this is how trackTitles looks ");
            for (size_t i = 0; i < trackTitles.size(); i++)
            {
                DBG(trackTitles[i]);
            }*/
            trackTitlesToURLs.erase(button->getComponentID().toStdString());                        
            trackTitlesToDuration.erase(button->getComponentID().toStdString());                                    
            
            

            //DBG("get num rows");
            //DBG(tableComponent.getNumRows());
            //tableComponent.updateContent();

            tableComponent.updateContent();
            int repaintRows = tableComponent.getNumRows();
            //DBG("get num rows");
            //DBG(tableComponent.getNumRows());
            
            //for (int i = 0; i < repaintRows; i++)
            //{
            //    for (size_t j = 0; j < componentsToUpdate.size(); j++)
            //    {
            //        refreshComponentForCell(i, 3, false, componentsToUpdate[j]);
            //        refreshComponentForCell(i, 4, false, componentsToUpdate[j]);
            //        refreshComponentForCell(i, 5, false, componentsToUpdate[j]);
            //    }
            //    //DBG("call repaint row");
            //    //DBG(i);
            //    /*refreshComponentForCell(i, 3, false, nullptr);
            //    refreshComponentForCell(i, 4, false, nullptr);
            //    refreshComponentForCell(i, 5, false, nullptr);*/
            //    //DBG((tableComponent.getNumRows() + 1));
            //    //tableComponent.repaintRow(i);
            //}
            //tableComponent.updateContent();

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

            ////update table view
            //tableComponent.updateContent();
            //for (size_t i = 1; i < (tableComponent.getNumRows()+1); i++)
            //{
            //    tableComponent.repaintRow(i);
            //}
            //tableComponent.updateContent();
            
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