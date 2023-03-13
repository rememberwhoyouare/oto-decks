/*
  ==============================================================================

    WaveformDisplay.cpp
    Created: 17 Jul 2022 12:22:30pm
    Author:  Polly

  ==============================================================================
*/

#include <JuceHeader.h>
#include "WaveformDisplay.h"
#include <math.h>


//==============================================================================
WaveformDisplay::WaveformDisplay(juce::AudioFormatManager & formatManagerToUse,
                                 juce::AudioThumbnailCache & cacheToUse) :
                                 audioThumb(1000, formatManagerToUse, cacheToUse),
                                 fileLoaded(false),
                                 position(0),
                                 seconds(0),
                                 mins(0),
                                 ssecs(0),
                                 millisecs(0)
                                 
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    audioThumb.addChangeListener(this);
}

WaveformDisplay::~WaveformDisplay()
{
}

void WaveformDisplay::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    g.setColour (juce::Colours::lightgreen);
    if (fileLoaded) {
        audioThumb.drawChannel(g, getLocalBounds(), 0, audioThumb.getTotalLength(), 0, 1.0);
        g.setColour(juce::Colours::black);
        g.drawRect(position * getWidth(), 0, 4, getHeight());
        g.setFont(20.0f);
//        display time in min::sec:::ms
        g.drawText(setTimeInSeconds(seconds), getLocalBounds(),juce::Justification::centred, true);
    } else {
        g.setFont(20.0f);
        g.drawText("File not loaded...", getLocalBounds(),
                    juce::Justification::centred, true);
    }

}

void WaveformDisplay::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void WaveformDisplay::loadURL(juce::URL audiURL) {
    audioThumb.clear();
    fileLoaded = audioThumb.setSource(new juce::URLInputSource(audiURL));
    if (fileLoaded) {
        std::cout << "wfd: loadURL" << std::endl;
        
    } else {
        std::cout << "wfd: error" << std::endl;
    }
}


void WaveformDisplay::changeListenerCallback(juce::ChangeBroadcaster *source) {
    
    repaint();
}


void WaveformDisplay::setPositionRelative(double pos) {
    if (pos != position) {
        position = pos;
        repaint();
    }
}

std::string WaveformDisplay::setTimeInSeconds(double secs) {

    if (secs != seconds) {
        seconds = secs;
    }
//    declare string variables for the output
    std::string minsStr;
    std::string secsStr;
    std::string millisecsStr;
//    convert time in seconds to minutes, seconds and milliseconds
    mins = int(seconds)/60;
    ssecs = int(seconds)%59;
    millisecs = int((seconds - int(seconds)) * 100);
    
//    display a "0" if number is less than 10
    if (mins < 10) {
        minsStr = "0" + std::to_string(mins);
    } else if (mins >= 10) {
        minsStr = std::to_string(mins);
    }
    if (ssecs < 10) {
        secsStr = "0" + std::to_string(ssecs);
    } else if (ssecs >= 10) {
        secsStr = std::to_string(ssecs);
    }
    if (millisecs < 10) {
        millisecsStr = "0" + std::to_string(millisecs);
    } else if (millisecs >= 10) {
        millisecsStr = std::to_string(millisecs);
    }
    
    return minsStr + ":" + secsStr + ":" + millisecsStr;
}


