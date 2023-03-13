/*
  ==============================================================================

    WaveformDisplay.h
    Created: 17 Jul 2022 12:22:30pm
    Author:  Polly

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay  : public juce::Component,
                         public juce::ChangeListener
{
public:
    WaveformDisplay(juce::AudioFormatManager & formatManagerToUse,
                    juce::AudioThumbnailCache & cacheToUse);
    ~WaveformDisplay() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    void changeListenerCallback(juce::ChangeBroadcaster *source) override;
    void loadURL(juce::URL audiURL);
    
    /** set the relative position of the playhead*/
    void setPositionRelative(double pos);
    /**display track time on the deck*/
    std::string setTimeInSeconds(double secs);

private:
    juce::AudioThumbnail audioThumb;
    bool fileLoaded;
    double position;
    double seconds;
    int mins;
    int ssecs;
    int millisecs;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveformDisplay)
};
