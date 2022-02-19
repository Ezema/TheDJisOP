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
DeckGUI::DeckGUI(AudioPlayer* _player, AudioFormatManager & formatManagerToUse, AudioThumbnailCache & cacheToUse, std::vector<URL>* trackFilesUrl, std::vector<std::string>* trackTitles/*,PlaylistComponent* playlistComponent*/, int _guiIdentifier) : player(_player),waveformDisplay(formatManagerToUse, cacheToUse), guiIdentifier(_guiIdentifier)
{   
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(playFromBeginningButton);
    addAndMakeVisible(loopSongButton);

    addAndMakeVisible(volumeLabel);
    addAndMakeVisible(volumeSlider);    
    
    addAndMakeVisible(playbackSpeedLabel);
    addAndMakeVisible(playbackSpeedSlider);
       
    addAndMakeVisible(positionLabel);    
    addAndMakeVisible(positionSlider);

    addAndMakeVisible(jump5secondsButton);
    addAndMakeVisible(goBack5secondsButton);   

    addAndMakeVisible(goBackOrJump5SecondsLabel);

    playButton.addListener(this);
    stopButton.addListener(this);    
    loopSongButton.addListener(this);    
    playFromBeginningButton.addListener(this);        


    playButton.setColour(TextButton::buttonColourId, Colours::green);
    stopButton.setColour(TextButton::buttonColourId, Colours::blue);
    playFromBeginningButton.setColour(TextButton::buttonColourId, Colours::darkviolet);
    loopSongButton.setColour(TextButton::buttonColourId, Colours::darkgrey);    

    goBackOrJump5SecondsLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    goBackOrJump5SecondsLabel.setText("go back/jump 5 seconds", juce::dontSendNotification);
    if (guiIdentifier == 1) {
        goBackOrJump5SecondsLabel.setColour(juce::Label::textColourId, juce::Colours::blue);
    };
    if (guiIdentifier == 2) {
        goBackOrJump5SecondsLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    };
    goBackOrJump5SecondsLabel.setJustificationType(juce::Justification::centred);

    /*if (guiIdentifier == 1) {
        goBack5secondsButton.setColour(ArrowButton::buttonColourId,)
    };
    if (guiIdentifier == 2) {
        volumeLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    };*/

    positionLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    positionLabel.setText("Position", juce::dontSendNotification);
    positionLabel.setJustificationType(Justification::centred);
    if (guiIdentifier == 1) {
        positionLabel.setColour(juce::Label::textColourId, juce::Colours::blue);
    };
    if (guiIdentifier == 2) {
        positionLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    };
    positionSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    if (guiIdentifier == 1) {
        positionSlider.setColour(Slider::textBoxTextColourId, Colours::black);
        positionSlider.setColour(Slider::thumbColourId, Colours::blue);
    };
    if (guiIdentifier == 2) {
        positionSlider.setColour(Slider::textBoxTextColourId, Colours::white);
        positionSlider.setColour(Slider::thumbColourId, Colours::orange);
    };
    positionSlider.setTextValueSuffix("%");
    positionSlider.setRange(0, 100, 1);
    positionSlider.setTextBoxStyle(Slider::TextBoxLeft, true, 50, 25);    
    positionSlider.setValue(0);
    
    
    
    playbackSpeedLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    playbackSpeedLabel.setText("Playback speed", juce::dontSendNotification);
    if (guiIdentifier == 1) {
        playbackSpeedLabel.setColour(juce::Label::textColourId, juce::Colours::blue);
    };
    if (guiIdentifier == 2) {
        playbackSpeedLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    };
    
    playbackSpeedLabel.setJustificationType(juce::Justification::centred);
    //positionSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);        
    playbackSpeedSlider.setTextBoxStyle(Slider::TextBoxLeft, true, 50, 25);
    playbackSpeedSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);

    if (guiIdentifier == 1) {
        playbackSpeedSlider.setColour(Slider::textBoxTextColourId, Colours::black);
    };
    if (guiIdentifier == 2) {
        playbackSpeedSlider.setColour(Slider::textBoxTextColourId, Colours::white);
    };


    playbackSpeedSlider.setTextValueSuffix("X");
    playbackSpeedSlider.setRange(0.01, 10, 0.01);
    playbackSpeedSlider.setTextBoxIsEditable(true);
    playbackSpeedSlider.setValue(1);


    volumeLabel.setFont(juce::Font(16.0f, juce::Font::bold));
    volumeLabel.setText("Volume", juce::dontSendNotification);
    if (guiIdentifier == 1) {
        volumeLabel.setColour(juce::Label::textColourId, juce::Colours::blue);
    };
    if (guiIdentifier == 2) {
        volumeLabel.setColour(juce::Label::textColourId, juce::Colours::orange);
    };    
    volumeLabel.setJustificationType(juce::Justification::centred);

    volumeSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
    volumeSlider.setTextValueSuffix("%");
    volumeSlider.setRange(0, 100, 1);
    if (guiIdentifier == 1) {
        volumeSlider.setColour(Slider::textBoxTextColourId, Colours::black);
        volumeSlider.setColour(Slider::thumbColourId, Colours::blue);
    };
    if (guiIdentifier == 2) {
        volumeSlider.setColour(Slider::textBoxTextColourId, Colours::white);
        volumeSlider.setColour(Slider::thumbColourId, Colours::orange);
    };
    volumeSlider.setTextBoxStyle(Slider::TextBoxLeft, true, 50, 25);
    volumeSlider.setValue(100);

    volumeSlider.addListener(this);
    playbackSpeedSlider.addListener(this);
    positionSlider.addListener(this);

    jump5secondsButton.addListener(this);
    goBack5secondsButton.addListener(this);
    
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

    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
    if (guiIdentifier==1) {
        g.fillAll(Colours::white);
    }
    if (guiIdentifier==2) {
        g.fillAll(Colours::black);
    }
    

    g.setColour (Colours::darkgrey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::white);
    g.setFont (14.0f);
    //g.drawText ("TheDJisOP", getLocalBounds(),Justification::centred, true);   // draw some placeholder text    
}

void DeckGUI::resized()
{
    float deckUIHeight = getHeight(); 
    float deckUIWidth = getWidth();
    float deckUIHeightTenth = deckUIHeight / 5;
    float deckUIWidthTenth = deckUIWidth / 10;
    
    playButton.setBounds((deckUIWidth-(deckUIWidthTenth * 8+30))/2, 5, deckUIWidthTenth * 2, deckUIHeightTenth);
    stopButton.setBounds((deckUIWidth - (deckUIWidthTenth * 8 + 30)) / 2 + 10 + deckUIWidthTenth * 2, 5, deckUIWidthTenth * 2, deckUIHeightTenth);
    playFromBeginningButton.setBounds((deckUIWidth - (deckUIWidthTenth * 8 + 30)) / 2 +20 + deckUIWidthTenth * 4, 5, deckUIWidthTenth * 2, deckUIHeightTenth);
    loopSongButton.setBounds((deckUIWidth - (deckUIWidthTenth * 8 + 30)) / 2  + 30 + deckUIWidthTenth * 6, 5, deckUIWidthTenth * 2, deckUIHeightTenth);
        

    goBackOrJump5SecondsLabel.setBounds(deckUIWidthTenth, deckUIHeightTenth * 1.2, deckUIWidthTenth*3, deckUIHeightTenth);
    
    goBack5secondsButton.setBounds(deckUIWidthTenth*1.25, deckUIHeightTenth * 2, deckUIWidthTenth * 1 , deckUIHeightTenth);
    jump5secondsButton.setBounds(deckUIWidthTenth*2.75, deckUIHeightTenth * 2, deckUIWidthTenth * 1, deckUIHeightTenth);


    positionLabel.setBounds(deckUIWidthTenth * 4.5, deckUIHeightTenth * 1.2, deckUIWidthTenth * 5, deckUIHeightTenth);
    positionSlider.setBounds(deckUIWidthTenth * 4.5, deckUIHeightTenth * 2, deckUIWidthTenth * 5, deckUIHeightTenth);

    
    playbackSpeedLabel.setBounds(deckUIWidthTenth * 1,deckUIHeightTenth*3, deckUIWidthTenth*3, deckUIHeightTenth);
    playbackSpeedSlider.setBounds(deckUIWidthTenth * 1, deckUIHeightTenth*3.8, deckUIWidthTenth * 3, deckUIHeightTenth);

    volumeLabel.setBounds(deckUIWidthTenth * 4.5, deckUIHeightTenth*3, deckUIWidthTenth * 5, deckUIHeightTenth);
    volumeSlider.setBounds((deckUIWidthTenth * 4.5), deckUIHeightTenth*3.8, deckUIWidthTenth *5, deckUIHeightTenth);
    

}

void DeckGUI::buttonClicked(Button* button)
{    
    if (button == &playButton)
    {    
        if (player->getLengthInSeconds() > 0) {
            isPlaying = true;
            playButton.setButtonText("PLAYING");
            stopButton.setButtonText("PAUSE");
            player->start();
        }
        
    }
     if (button == &stopButton)
    {   
         if (player->getLengthInSeconds() > 0) {
             isPlaying = false;
             playButton.setButtonText("PLAY");
             stopButton.setButtonText("PAUSED");
             player->stop();
         }

    }
    if (button == &playFromBeginningButton) {        
        if (player->getLengthInSeconds() > 0) {
            playButton.setButtonText("PLAYING");
            stopButton.setButtonText("PAUSE");
            player->setPositionRelative(0);
            player->start();
        }
    }
    if (button == &loopSongButton) {
        
        loopEnabled = !loopEnabled;        
        if (loopEnabled) {
            button->setButtonText("LOOP ENABLED");
            button->setColour(TextButton::buttonColourId, Colours::red);
            button->setColour(TextButton::textColourOnId, Colours::black);
            button->setColour(TextButton::textColourOffId, Colours::black);
        }else {
            button->setButtonText("LOOP DISABLED");
            button->setColour(TextButton::buttonColourId, Colours::darkgrey);
            button->setColour(TextButton::textColourOnId, Colours::white);
            button->setColour(TextButton::textColourOffId, Colours::white);
        }
    }    
    if (button == &goBack5secondsButton) {        
        
        auto currentPositionInSeconds = player->getLengthInSeconds() * player->getPositionRelative()/100;
        
        auto minus5SecondsPositionInSeconds = currentPositionInSeconds - 5;
        
        player->setPositionRelative(minus5SecondsPositionInSeconds / (player->getLengthInSeconds()));
    }
    if (button == &jump5secondsButton) {               
        
        auto currentPositionInSeconds = player->getLengthInSeconds() * player->getPositionRelative()/100;
        
        auto plus5SecondsPositionInSeconds = currentPositionInSeconds + 5;
        
        player->setPositionRelative(plus5SecondsPositionInSeconds / (player->getLengthInSeconds()));
    }
}

void DeckGUI::loadTrack(std::vector<URL> trackFilesUrl, int index)
{
    FileChooser chooser{ "Select a file..." };
    if (chooser.browseForFileToOpen())
    {
        player->loadURL(URL{ chooser.getResult() });
        waveformDisplay.loadURL(URL{ chooser.getResult() });

    }
    
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