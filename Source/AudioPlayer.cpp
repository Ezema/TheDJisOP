#include "AudioPlayer.h"

using namespace juce;

AudioPlayer::AudioPlayer(AudioFormatManager& _formatManager, std::vector<URL>* trackFilesUrl, std::vector<std::string>* trackTitles)
: formatManager(_formatManager)
{
    addAndMakeVisible(customVisualizer);
    currentVolume = 0;
}
AudioPlayer::~AudioPlayer()
{

}

void AudioPlayer::resized() {

    customVisualizer.setBounds(0, 0, getWidth()/10*8, getHeight());

}
void AudioPlayer::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);

    g.setColour(juce::Colours::black);
    g.setFont(14.0f);

    g.setOpacity(1);
    g.setColour(Colours::black);    

    if (currentVolume <= 0.7) {
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
    auto auxiliary = bufferToFill.buffer->getMagnitude(bufferToFill.startSample, bufferToFill.numSamples);
    magnitude.store(auxiliary);

    // save the current relative volume and repaint
    currentVolume = magnitude;
    repaint();
    
}
void AudioPlayer::releaseResources()
{
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void AudioPlayer::loadURL(URL audioURL)
{    
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));   
    if (reader != nullptr)
    {               
        std::unique_ptr<AudioFormatReaderSource> newSource (new AudioFormatReaderSource (reader,true)); 
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);             
        readerSource.reset(newSource.release());
    }
}
void AudioPlayer::setGain(double gain)
{
    if (gain < 0 || gain > 1.0)
    {

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