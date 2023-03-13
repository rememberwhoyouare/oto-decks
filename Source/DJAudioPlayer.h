/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 12 Jul 2022 6:00:27pm
    Author:  Polly

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class DJAudioPlayer : public juce::AudioSource {
    
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();
    
    
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;
    
    /** read a file into the app*/
    void loadURL(juce::URL audioURL);
    /**change the audio volume*/
    void setGain(double gain);
    /**change the audio speed*/
    void setSpeed(double ratio);
    /**change the audio position*/
    void setPosition(double posInSecs);
    /**change the audio position relatively to its length*/
    void setPositionRelative(double pos);
    /**start playing the audio file*/
    void start();
    /**stop playing the audio file*/
    void stop();
    /**loop the audio file*/
    void loop(bool loopActive);
    /**get the relative position of the playhead*/
    double getPositionRelative();
    /**get time in seconds*/
    double getTimeInSeconds();
    
    
    

private:
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource transportSource;
    juce::ResamplingAudioSource resampleSource{&transportSource, false, 2};
    
};
