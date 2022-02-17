/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "DeckGUI.h"
//#include "PlaylistComponent.h"

using namespace juce;

//==============================================================================
DeckGUI::DeckGUI(AudioPlayer* _player, AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse, std::vector<URL>* trackFilesUrl, std::vector<std::string>* trackTitles/*,PlaylistComponent* playlistComponent*/) : player(_player),
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    //trackTitles = trackTitles;
    //trackFilesUrl = trackFilesUrl;

    //playlistComponent = playlistComponent;

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(playFromBeginningButton);
    addAndMakeVisible(loopSongButton);
    //addAndMakeVisible(nextTrackButton);
    //addAndMakeVisible(loadButton);
       
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(volumeLabel);
    
    addAndMakeVisible(playbackSpeedLabel);
    addAndMakeVisible(playbackSpeedSlider);
       
    addAndMakeVisible(positionLabel);    
    addAndMakeVisible(positionSlider);

    /*addAndMakeVisible(testSliderOne);
    addAndMakeVisible(testSliderTwo);
    addAndMakeVisible(testSliderThree);
    addAndMakeVisible(testSliderFour);
    addAndMakeVisible(testSliderFive);*/

    //addAndMakeVisible(waveformDisplay);

    playButton.addListener(this);
    stopButton.addListener(this);    
    loopSongButton.addListener(this);    
    playFromBeginningButton.addListener(this);


    playButton.setColour(TextButton::buttonColourId, Colours::green);
    stopButton.setColour(TextButton::buttonColourId, Colours::blue);

    //nextTrackButton.addListener(this);

    // Sliders and buttons configuration ==============

    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::red);

    positionLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    positionLabel.setText("Position", juce::dontSendNotification);
    positionLabel.setColour(juce::Label::textColourId, juce::Colours::yellow);
    positionLabel.setJustificationType(juce::Justification::centred);
    
    positionSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    positionSlider.setTextValueSuffix("%");
    positionSlider.setRange(0, 100, 1);
    positionSlider.setTextBoxStyle(Slider::TextBoxLeft, true, 50, 25);    
    positionSlider.setValue(0);
    
    
    
    playbackSpeedLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    playbackSpeedLabel.setText("Playback speed", juce::dontSendNotification);
    playbackSpeedLabel.setColour(juce::Label::textColourId, juce::Colours::yellow);
    playbackSpeedLabel.setJustificationType(juce::Justification::centred);
    //positionSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);        
    playbackSpeedSlider.setTextBoxStyle(Slider::TextBoxLeft, true, 50, 25);
    playbackSpeedSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    playbackSpeedSlider.setTextValueSuffix("X");
    playbackSpeedSlider.setRange(0.01, 10, 0.01);
    playbackSpeedSlider.setTextBoxIsEditable(true);
    playbackSpeedSlider.setValue(1);


    volumeLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    volumeLabel.setText("Volume", juce::dontSendNotification);
    volumeLabel.setColour(juce::Label::textColourId, juce::Colours::yellow);
    volumeLabel.setJustificationType(juce::Justification::centred);

    volumeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    volumeSlider.setTextValueSuffix("%");
    volumeSlider.setRange(0, 100, 1);
    volumeSlider.setTextBoxStyle(Slider::TextBoxLeft, true, 50, 25);
    volumeSlider.setValue(20);

    volumeSlider.addListener(this);
    playbackSpeedSlider.addListener(this);
    positionSlider.addListener(this);

    //volumeSlider.setRange(0.0, 1.0);
    /*playbackSpeedSlider.setRange(0.0, 10.0);
    positionSlider.setRange(0.0, 1.0);    */

    // Sliders and buttons configuration ==============

    loopEnabled = false;

    startTimer(2000);


}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::white);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    g.drawText ("TheDJisOP", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text    
}

void DeckGUI::resized()
{
    float deckUIHeight = getHeight(); 
    float deckUIWidth = getWidth();
    float deckUIHeightTenth = deckUIHeight / 10;
    float deckUIWidthTenth = deckUIWidth / 10;
    
    //waveformDisplay.setBounds(0, 0, deckUIWidth, deckUIHeightTenth * 3);
    //loadButton.setBounds(0, deckUIHeightTenth * 4, getWidth(), deckUIHeightTenth * 6);
    
    /*playButton.setBounds(0, 0, deckUIWidthTenth*2, deckUIHeightTenth);
    stopButton.setBounds(deckUIWidthTenth * 2, 0, deckUIWidthTenth * 2, deckUIHeightTenth);
    playFromBeginningButton.setBounds((deckUIWidthTenth * 2)*2, 0, deckUIWidthTenth * 2, deckUIHeightTenth);*/

    /*playButton.setBounds(deckUIWidthTenth, 5, deckUIWidthTenth * 2, deckUIHeightTenth);
    stopButton.setBounds(deckUIWidthTenth * 4, 5, deckUIWidthTenth * 2, deckUIHeightTenth);
    playFromBeginningButton.setBounds(deckUIWidthTenth *7, 5, deckUIWidthTenth * 2, deckUIHeightTenth);*/    
    
    DBG("deckUIWidthTenth * (2 / 5)");
    DBG(deckUIWidthTenth * (2 / 5));
    playButton.setBounds((deckUIWidth-(deckUIWidthTenth * 8+30))/2, 5, deckUIWidthTenth * 2, deckUIHeightTenth);
    stopButton.setBounds((deckUIWidth - (deckUIWidthTenth * 8 + 30)) / 2 + 10 + deckUIWidthTenth * 2, 5, deckUIWidthTenth * 2, deckUIHeightTenth);
    playFromBeginningButton.setBounds((deckUIWidth - (deckUIWidthTenth * 8 + 30)) / 2 +20 + deckUIWidthTenth * 4, 5, deckUIWidthTenth * 2, deckUIHeightTenth);
    loopSongButton.setBounds((deckUIWidth - (deckUIWidthTenth * 8 + 30)) / 2  + 30 + deckUIWidthTenth * 6, 5, deckUIWidthTenth * 2, deckUIHeightTenth);
        
    
    playbackSpeedLabel.setBounds(10,deckUIHeightTenth*2, deckUIWidthTenth*3, deckUIHeightTenth);
    playbackSpeedSlider.setBounds(10, deckUIHeightTenth*3, deckUIWidthTenth * 3, deckUIHeightTenth);

    volumeLabel.setBounds((deckUIWidthTenth * 3.5)+10, deckUIHeightTenth*2, deckUIWidthTenth * 3, deckUIHeightTenth);
    volumeSlider.setBounds((deckUIWidthTenth * 3.5)+10, deckUIHeightTenth*3, deckUIWidthTenth * 3, deckUIHeightTenth);
    
    positionLabel.setBounds((deckUIWidthTenth * 6.7)+10, deckUIHeightTenth*2, deckUIWidthTenth * 3, deckUIHeightTenth);
    positionSlider.setBounds((deckUIWidthTenth * 6.7) + 10, deckUIHeightTenth * 3, deckUIWidthTenth * 3, deckUIHeightTenth);

    

    //playbackSpeedSlider.setBounds(deckUIHeightTenth * 4.5, deckUIHeightTenth, deckUIWidthTenth * 4, deckUIHeightTenth * 4.5);
    //positionSlider.setBounds(0, deckUIHeightTenth * 5.5, deckUIWidthTenth * 4, deckUIHeightTenth * 4.5);        
}

void DeckGUI::buttonClicked(Button* button)
{    
    if (button == &playButton)
    {    
        std::cout << "Play button was clicked " << std::endl;
        player->start();
    }
     if (button == &stopButton)
    {     
        std::cout << "Stop button was clicked " << std::endl;
        player->stop();

    }
    if (button == &playFromBeginningButton) {        
        player->setPositionRelative(0);
        player->start();
    }
    if (button == &loopSongButton) {
        
        loopEnabled = !loopEnabled;        
        if (loopEnabled) {
            button->setColour(TextButton::buttonColourId, Colours::red);
            button->setColour(TextButton::textColourOnId, Colours::black);
            button->setColour(TextButton::textColourOffId, Colours::black);
        }else {
            button->setColour(TextButton::buttonColourId, Colours::darkgrey);
            button->setColour(TextButton::textColourOnId, Colours::white);
            button->setColour(TextButton::textColourOffId, Colours::white);
        }
    }
    //if (button == &nextTrackButton) {
    //    //playlistComponent->handlePlayNextTrack("left");
    //}
    
}

void DeckGUI::loadTrack(std::vector<URL> trackFilesUrl, int index)
{
    FileChooser chooser{ "Select a file..." };
    if (chooser.browseForFileToOpen())
    {
        player->loadURL(URL{ chooser.getResult() });
        waveformDisplay.loadURL(URL{ chooser.getResult() });

    }
    //DBG("INDEX is: "+std::to_string(index));
    //URL urlInstance = trackFilesUrl[index];
    //DBG(urlInstance.getDomain().toStdString());    
    //player->loadURL(urlInstance);
    //waveformDisplay.loadURL(urlInstance);    
   
    
}

void DeckGUI::sliderValueChanged (Slider *slider)
{
    if (slider == &volumeSlider)
    {
        player->setGain(slider->getValue()/100);
    }

    if (slider == &playbackSpeedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &positionSlider)
    {
        if ((slider->getValue() - player->getPositionRelative()) > 2) {
            player->setPositionRelative(slider->getValue()/100);
        }
        else if (player->getPositionRelative() - slider->getValue() > 2) {
            player->setPositionRelative(slider->getValue()/100);
        }
        
    }
    
}

bool DeckGUI::isInterestedInFileDrag (const StringArray &files)
{
  std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
  return true; 
}

void DeckGUI::filesDropped (const StringArray &files, int x, int y)
{
  std::cout << "DeckGUI::filesDropped" << std::endl;
  if (files.size() == 1)
  {
    player->loadURL(URL{File{files[0]}});
  }
}

void DeckGUI::timerCallback()
{  
    positionSlider.setValue(player->getPositionRelative());

    if (loopEnabled) {
        if (player->getPositionRelative() >=100) {
            player->setPositionRelative(0);
            player->start();
        }
    }
}