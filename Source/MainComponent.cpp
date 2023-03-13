#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (1000, 750);

    // Some platforms require permissions to open input channels so request that here
    if (juce::RuntimePermissions::isRequired (juce::RuntimePermissions::recordAudio)
        && ! juce::RuntimePermissions::isGranted (juce::RuntimePermissions::recordAudio))
    {
        juce::RuntimePermissions::request (juce::RuntimePermissions::recordAudio,
                                           [&] (bool granted) { setAudioChannels (granted ? 2 : 0, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    addAndMakeVisible(deckGUI1);
    addAndMakeVisible(deckGUI2);
    
    addAndMakeVisible(playlistComponent);
    
    formatManager.registerBasicFormats();
    
    startTimer(500);

}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
    stopTimer();
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

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    mixerSource.getNextAudioBlock(bufferToFill);
}

void MainComponent::releaseResources()
{
    mixerSource.removeAllInputs();
    mixerSource.releaseResources();
    player1.releaseResources();
    player2.releaseResources();
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//    g.fillAll (juce::Colours::lightskyblue);

    // You can add your drawing code here!
    
}

void MainComponent::resized()
{
    deckGUI1.setBounds(0, 0, getWidth()/2, getHeight()/2 + 100);
    deckGUI2.setBounds(getWidth()/2, 0, getWidth()/2, getHeight()/2 + 100);
    
    playlistComponent.setBounds(0, getHeight()/2 + 100, getWidth(), getHeight());
}



void MainComponent::timerCallback() { //Function loops every 500ms, checks if user has sent an audio file into a deckGUI

    //open playlist.txt and read lines
    std::ifstream file("/Users/polly/Desktop/test/playlist.txt");
    std::string str;
    std::vector<std::string> lines;
    while (std::getline(file, str))
    {
        lines.push_back(str);
    }
    file.close();
    
    //if user sent files to deck
    if (std::stoi(lines[1]) == 1) {
        deckGUI1.loadFromPlaylist(); //plays audio file in deckGUI1
        std::ofstream myfile("/Users/polly/Desktop/test/playlist.txt");

        myfile << "" << std::endl << "0" << std::endl; //overwrite all the text file into 0, so the function stops looping
        myfile.close(); //closes file
    }
    else if (std::stoi(lines[1]) == 2) { 
        deckGUI2.loadFromPlaylist(); //plays audio file in deckGUI2
        std::ofstream myfile("/Users/polly/Desktop/test/playlist.txt");
        myfile << "" << std::endl << "0" << std::endl;  //overwrite all the text file into 0, so the function stops looping
        myfile.close(); //closes file
    }
    
}

