/*
  ==============================================================================

    DeckGUI.h
    Created: 14 Jul 2022 1:32:34pm
    Author:  Polly

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"
#include "PlaylistComponent.h"

//==============================================================================
/*
*/
class DeckGUI  : public juce::Component,
                public juce::Button::Listener,
                public juce::Slider::Listener,
                public juce::FileDragAndDropTarget,
                public juce::Timer
              

{
public:
    DeckGUI(DJAudioPlayer* player,
                            juce::AudioFormatManager & formatManagerToUse,
                            juce::AudioThumbnailCache & cacheToUse);
    ~DeckGUI() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    /** implement Button::Listener */
    void buttonClicked (juce::Button* button) override;
    /** implement Slider::Listener */
    void sliderValueChanged(juce::Slider *slider) override;
    
    bool isInterestedInFileDrag (const juce::StringArray &files) override;
    void filesDropped (const juce::StringArray &files, int x, int y) override;
    void timerCallback() override;
    /**loads tracks from playlist to GUI*/
    void loadFromPlaylist();
    



private:
    
//    implement buttons and sliders
    juce::TextButton playButton{"PLAY"};
    juce::TextButton stopButton{"STOP"};
//    juce::TextButton cueButton{"CUE"};
    juce::TextButton loopButton{"LOOP"};
//    juce::TextButton prevButton{"PREV"};
//    juce::TextButton nextButton{"NEXT"};

    juce::Slider volSlider;
    juce::Slider speedSlider;
    juce::Slider posSlider;
    
    juce::String trackName;
    
    DJAudioPlayer* player;
    WaveformDisplay waveformDisplay;
    
//   check if the loop button is active
    bool loopActive;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DeckGUI)
};
