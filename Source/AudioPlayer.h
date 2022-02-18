/*
  ==============================================================================

    AudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  matthew

  ==============================================================================
*/

#pragma once

#include "JuceHeader.h"
#include "AudioVisualizer.h"

using namespace juce;

class AudioPlayer : public AudioSource, public Component{
  public:

    AudioPlayer(AudioFormatManager& _formatManager, std::vector<URL>* trackFilesUrl, std::vector<std::string>* trackTitles);
    ~AudioPlayer();

    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(URL audioURL);
    void setGain(double gain);
    void setSpeed(double ratio);
    void setPosition(double posInSecs);
    void setPositionRelative(double pos);

    double getLengthInSeconds();
    

    void start();
    void stop();

    /** get the relative position of the playhead */
    double getPositionRelative();

    void paint(Graphics&) override;
    void resized() override;

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;    
    AudioTransportSource transportSource; 
    AudioTransportSource reverseTransportSource;
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
    ResamplingAudioSource reverseResampleSource{ &reverseTransportSource, false, 2 };

    CustomVisualizer customVisualizer;
    //CustomVisualizer customVisualizer2;

};




