#pragma once

#include "JuceHeader.h"
#include "AudioVisualizer.h"

using namespace juce;

class AudioPlayer : public AudioSource, public Component{
  public:

    AudioPlayer(AudioFormatManager& _formatManager, std::vector<URL>* trackFilesUrl, std::vector<std::string>* trackTitles);
    ~AudioPlayer();    

    // virtual functions inherited from JUCE AudioSource class
    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    // load the song URL into the player
    void loadURL(URL audioURL);
    // set the gain (volume percentage) of the playback
    void setGain(double gain); 
    // set the speed of the playback
    void setSpeed(double ratio);
    //  set the absolute position (seconds) of the playback
    void setPosition(double posInSecs);
    //  set the relative position (percentage) of the playback
    void setPositionRelative(double pos);
    //  get the length of the song in seconds
    double getLengthInSeconds();       
    // start playback
    void start();
    // stop playback
    void stop();
    // get relative position of the playback
    double getPositionRelative();

private:

    // virtual functions inherited from JUCE Componentclass
    void paint(Graphics&) override;
    void resized() override;
    
    // The following will be used to implement the playback
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;    
    AudioTransportSource transportSource; 
    AudioTransportSource reverseTransportSource;
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
    ResamplingAudioSource reverseResampleSource{ &reverseTransportSource, false, 2 };

    // Instance of the CustomVisualizer to visualize the frequency of the audio file
    CustomVisualizer customVisualizer;

    // To temporary store the relative volume of the buffer across the song
    std::atomic<float> magnitude{ 0.0f };    
    double currentVolume;
};




