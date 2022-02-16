/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

using namespace juce;

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1280, 720);

    //myPointer = &playlistComponent;    

    /*player1 = new AudioPlayer{ formatManager, &trackFilesUrl, &trackTitles };*/
    //deckGUI1{ &player1, formatManager, thumbCache, &trackFilesUrl, &trackTitles/*, &playlistComponent*/ };
    //AudioPlayer player2{ formatManager ,&trackFilesUrl, &trackTitles };
    //DeckGUI deckGUI2{ &player2, formatManager, thumbCache, &trackFilesUrl, &trackTitles/*, &playlistComponent*/ };
    //PlaylistComponent playlistComponent{ &player1,&player2,&deckGUI1 ,&deckGUI2, formatManager, thumbCache };

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }  
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (0, 2);
    }  

    addAndMakeVisible(playlistComponent);
    addAndMakeVisible(deckGUI1); 
    addAndMakeVisible(deckGUI2);  
    


    formatManager.registerBasicFormats();
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
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

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{ 
    int windowTenth= getHeight() / 10;
    playlistComponent.setBounds(0, 0, getWidth(), windowTenth*6);
    deckGUI1.setBounds(0, windowTenth *6, getWidth()/2, windowTenth *4);
    deckGUI2.setBounds(getWidth()/2, windowTenth *6, getWidth()/2, windowTenth *4);
    
}

