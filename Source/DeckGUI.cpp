/*
  ==============================================================================

    DeckGUI.cpp
    Created: 13 Mar 2020 6:44:48pm
    Author:  matthew

  ==============================================================================
*/

#include "JuceHeader.h"
#include "DeckGUI.h"

using namespace juce;

//==============================================================================
DeckGUI::DeckGUI(AudioPlayer* _player, AudioFormatManager & formatManagerToUse,         AudioThumbnailCache & cacheToUse, std::vector<URL>* trackFilesUrl, std::vector<std::string>* trackTitles) : player(_player),
               waveformDisplay(formatManagerToUse, cacheToUse)
{

    //trackTitles = trackTitles;
    //trackFilesUrl = trackFilesUrl;

    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
    addAndMakeVisible(fastForwButton);
    addAndMakeVisible(nextTrackButton);
    //addAndMakeVisible(loadButton);
       
    addAndMakeVisible(volumeSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(positionSlider);

    /*addAndMakeVisible(testSliderOne);
    addAndMakeVisible(testSliderTwo);
    addAndMakeVisible(testSliderThree);
    addAndMakeVisible(testSliderFour);
    addAndMakeVisible(testSliderFive);*/

    //addAndMakeVisible(waveformDisplay);

    playButton.addListener(this);
    stopButton.addListener(this);
    loadButton.addListener(this);

    // Sliders and buttons configuration ==============

    volumeSlider.setSliderStyle(Slider::SliderStyle::Rotary);    
    speedSlider.setSliderStyle(Slider::SliderStyle::ThreeValueVertical);
    positionSlider.setSliderStyle(Slider::SliderStyle::IncDecButtons);
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::red);
    volumeSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);
    speedSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 100, 25);

    volumeSlider.addListener(this);
    speedSlider.addListener(this);
    positionSlider.addListener(this);

    volumeSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 100.0);
    positionSlider.setRange(0.0, 1.0);

    // Sliders and buttons configuration ==============

    //startTimer(500);


}

DeckGUI::~DeckGUI()
{
    //stopTimer();
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
    
    playButton.setBounds(0, 0, deckUIWidthTenth*2, deckUIHeightTenth);
    stopButton.setBounds(deckUIWidthTenth * 2, 0, deckUIWidthTenth * 2, deckUIHeightTenth);
    fastForwButton.setBounds((deckUIWidthTenth * 2)*2, 0, deckUIWidthTenth * 2, deckUIHeightTenth);
    nextTrackButton.setBounds((deckUIWidthTenth * 2) * 3, 0, deckUIWidthTenth * 2, deckUIHeightTenth);

    
    volumeSlider.setBounds(0, deckUIHeightTenth, deckUIWidthTenth*4, deckUIHeightTenth * 4.5);
    speedSlider.setBounds(deckUIHeightTenth * 4.5, deckUIHeightTenth, deckUIWidthTenth * 4, deckUIHeightTenth * 4.5);
    positionSlider.setBounds(0, deckUIHeightTenth * 5.5, deckUIWidthTenth * 4, deckUIHeightTenth * 4.5);        
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
    if (button == &loadButton)
    {        
        FileChooser chooser{"Select a file..."};
        if (chooser.browseForFileToOpen())
        {
            player->loadURL(URL{chooser.getResult()});
            waveformDisplay.loadURL(URL{chooser.getResult()});
            
        }
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
        player->setGain(slider->getValue());
    }

    if (slider == &speedSlider)
    {
        player->setSpeed(slider->getValue());
    }
    
    if (slider == &positionSlider)
    {
        player->setPositionRelative(slider->getValue());
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

//void DeckGUI::timerCallback()
//{
//    //std::cout << "DeckGUI::timerCallback" << std::endl;
//    waveformDisplay.setPositionRelative(
//            player->getPositionRelative());
//}


    

