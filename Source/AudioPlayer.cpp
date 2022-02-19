/*
==============================================================================

AudioPlayer.cpp
Created: 
Author:  

==============================================================================
*/

#include "AudioPlayer.h"

using namespace juce;

AudioPlayer::AudioPlayer(AudioFormatManager& _formatManager, std::vector<URL>* trackFilesUrl, std::vector<std::string>* trackTitles)
: formatManager(_formatManager)
{
    addAndMakeVisible(customVisualizer);
    startTimer(1);
    currentVolume = 0;
    previousVolume = 0;
}
AudioPlayer::~AudioPlayer()
{
    stopTimer();
}

void AudioPlayer::resized() {

    customVisualizer.setBounds(0, 0, getWidth()/10*8, getHeight());

}
void AudioPlayer::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(juce::Colours::black);   // clear the background

    g.setColour(juce::Colours::black);
    g.drawRect(getLocalBounds(), 1);   // draw an outline around the component

    g.setColour(juce::Colours::black);
    g.setFont(14.0f);

    g.setOpacity(1);
    g.setColour(Colours::black);
    int result = magnitude/*.load()*/;
    //result = (int)result;


    //DBG(previousVolume);
    //DBG(currentVolume);
    
    if (currentVolume>previousVolume) {        
        g.setColour(Colours::red);
        g.fillRect((getWidth() / 10 * 8), getHeight()-(getHeight() * currentVolume), (getWidth() / 10) * 2, (- 1));        
        lastKnownY = getHeight() - (getHeight() * currentVolume);
    }
    else {
        g.fillRect((getWidth() / 10 * 8), lastKnownY, (getWidth() / 10) * 2, (-1));
    }

    if (currentVolume < 0.7) {
        g.setColour(Colours::limegreen);
    }
    if (currentVolume > 0.7) {
        g.setColour(Colours::yellow);
    }
    if (currentVolume > 0.85) {
        g.setColour(Colours::red);
    }


    g.fillRect((getWidth() / 10 * 8) +  getWidth() / 30 , getHeight(), (getWidth() / 20), (- 1) * (getHeight() * currentVolume));

    g.fillRect((getWidth() / 10 * 8) + getWidth() / 30 + (getWidth() / 20) + getWidth() / 30, getHeight(), (getWidth() / 20), (-1) * (getHeight() * currentVolume));

    //This is working
    //g.fillRect((getWidth() / 10 * 8), 0, getWidth() / 10 * 2 * currentVolume, getHeight());
    
    //g.setColour(Colours::darkred);
}

void AudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) 
{
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    customVisualizer.clear();
}
void AudioPlayer::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{    
    resampleSource.getNextAudioBlock(bufferToFill);   
    customVisualizer.pushBuffer(bufferToFill);
    auto mag = bufferToFill.buffer->getMagnitude(bufferToFill.startSample, bufferToFill.numSamples);
    magnitude.store(mag);    
    //DBG("magnitude");
    //DBG(magnitude.load());
}
void AudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void AudioPlayer::loadURL(URL audioURL)
{    
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));   
    if (reader != nullptr) // good file!
    {               
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader,true)); 
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);             
        readerSource.reset(newSource.release());
    }

    //auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
    //if (reader != nullptr) // good file!
    //{
    //    std::unique_ptr<AudioFormatReaderSource> newSource(new AudioFormatReaderSource(reader, true));
    //    reverseTransportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
    //    readerSource.reset(newSource.release());
    //}
}
void AudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {
        std::cout << "AudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    }
    else {
        transportSource.setGain(gain);
    }
   
}
void AudioPlayer::setSpeed(double ratio)
{
  if (ratio < 0 || ratio > 100.0)
    {        
    }
    else {
        resampleSource.setResamplingRatio(ratio);
    }
}
void AudioPlayer::setPosition(double posInSecs)
{
    transportSource.setPosition(posInSecs);
    
}

void AudioPlayer::setPositionRelative(double pos)
{
     if (pos < 0 || pos > 1.0)
    {
    }
    else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}

void AudioPlayer::timerCallback() {
    currentVolume = magnitude;
    repaint();
    //previousVolume = magnitude;
    if (stepsCounter % 2 == 0) {
        previousVolume = magnitude;
    }
    stepsCounter = stepsCounter + 1;
    
}


void AudioPlayer::start()
{
    transportSource.start();    
}
void AudioPlayer::stop()
{
  transportSource.stop();
}


double AudioPlayer::getLengthInSeconds() {
    return transportSource.getLengthInSeconds();
}


double AudioPlayer::getPositionRelative()
{    
    if (transportSource.getLengthInSeconds() > 0) {
        return (transportSource.getCurrentPosition() / transportSource.getLengthInSeconds())*100;
    }
    else {
        return 0;
    };
    
}