#pragma once

#include "JuceHeader.h"
#include "AudioVisualizer.h"

using namespace juce;

class AudioPlayer : public AudioSource, public Component{
  public:

    AudioPlayer(AudioFormatManager& _formatManager, std::vector<URL>* trackFilesUrl, std::vector<std::string>* trackTitles);
    ~AudioPlayer();

    void paint(Graphics&) override;
    void resized() override;
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

    double getPositionRelative();

private:
    AudioFormatManager& formatManager;
    std::unique_ptr<AudioFormatReaderSource> readerSource;    
    AudioTransportSource transportSource; 
    AudioTransportSource reverseTransportSource;
    ResamplingAudioSource resampleSource{&transportSource, false, 2};
    ResamplingAudioSource reverseResampleSource{ &reverseTransportSource, false, 2 };

    CustomVisualizer customVisualizer;
    std::atomic<float> magnitude{ 0.0f };
    
    int stepsCounter = 1;
    double lastKnownY;
    double previousVolume;
    double currentVolume;

};




