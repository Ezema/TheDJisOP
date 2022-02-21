#include "MainComponent.h"

using namespace juce;

MainComponent::MainComponent()
{    
    setSize (1280, 720);

    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(playlistComponent);
    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);  
    addAndMakeVisible(player1);
    addAndMakeVisible(player2);
    
    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{    
    shutdownAudio();
}

void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    player1.prepareToPlay(samplesPerBlockExpected, sampleRate);
    player2.prepareToPlay(samplesPerBlockExpected, sampleRate);
    
    mixerSource.prepareToPlay(samplesPerBlockExpected, sampleRate);

    mixerSource.addInputSource(&player1, false);
    mixerSource.addInputSource(&player2, false);

 }
void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
    player1.releaseResources();
    player2.releaseResources();
    mixerSource.releaseResources();
}

void MainComponent::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));    
}

void MainComponent::resized()
{ 
    int windowTenth= getHeight() / 10;
    playlistComponent.setBounds(0, 0, getWidth(), windowTenth*5);
    player1.setBounds(0, windowTenth * 5, getWidth() / 2, windowTenth * 2.25);
    player2.setBounds(getWidth() / 2,windowTenth * 5, getWidth() / 2, windowTenth * 2.25);
    deckGUI1.setBounds(0, windowTenth *7.25, getWidth()/2, windowTenth *2.75);
    deckGUI2.setBounds(getWidth()/2, windowTenth * 7.25, getWidth()/2, windowTenth *2.75);
    
}

