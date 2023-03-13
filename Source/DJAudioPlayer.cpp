/*
  ==============================================================================

    DJAudioPlayer.cpp
    Created: 12 Jul 2022 6:00:27pm
    Author:  Polly

  ==============================================================================
*/

#include "DJAudioPlayer.h"

DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager) : formatManager(_formatManager){

}
DJAudioPlayer::~DJAudioPlayer() {
    
}


void DJAudioPlayer::prepareToPlay (int samplesPerBlockExpected, double sampleRate) {
    
    transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
    resampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill) {
    if (readerSource.get() == nullptr) {
        bufferToFill.clearActiveBufferRegion();
        return;
    }
    resampleSource.getNextAudioBlock(bufferToFill);
    
}

void DJAudioPlayer::releaseResources() {
    transportSource.releaseResources();
    resampleSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL) {
    auto* reader = formatManager.createReaderFor(audioURL.createInputStream(juce::URL::InputStreamOptions
                                                                            {juce::URL::ParameterHandling::inAddress}));
    if (reader != nullptr) {
        std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource (reader, true));
        transportSource.setSource (newSource.get(), 0, nullptr, reader->sampleRate);
        readerSource.reset (newSource.release());
        
    }
    else {
        std::cout << "Something went wrong loading the file " << std::endl;
    }
    
}

void DJAudioPlayer::setGain(double gain) {
    if (gain < 0 || gain > 1.0) {
        std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
    } else {
        transportSource.setGain(gain);
    }

}

void DJAudioPlayer::setSpeed(double ratio) {
    if (ratio < 0 || ratio > 100.0) {
        std::cout << "DJAudioPlayer::setSpeed ratio should be between 1 and 100" << std::endl;
    } else {
        resampleSource.setResamplingRatio(ratio);
    }
}

void DJAudioPlayer::setPosition(double posInSecs) {
    transportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos) {
    if (pos < 0 || pos > 1) {
        std::cout << "DJAudioPlayer::setPositionRelative poa should be between 0 and 1" << std::endl;
    } else {
        double posInSecs = transportSource.getLengthInSeconds() * pos;
        setPosition(posInSecs);
    }
}


void DJAudioPlayer::start() {
    transportSource.start();
}

void DJAudioPlayer::stop() {
    transportSource.stop();
}

void DJAudioPlayer::loop(bool loopActive) {
    if (loopActive == false) {
        readerSource->setLooping(true);
    } else {

        readerSource->setLooping(false);
    }
}

double DJAudioPlayer::getPositionRelative() {
    return transportSource.getCurrentPosition() / transportSource.getLengthInSeconds();
}

double DJAudioPlayer::getTimeInSeconds() {
    return transportSource.getCurrentPosition();
}
