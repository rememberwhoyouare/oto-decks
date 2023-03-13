/*
  ==============================================================================

    DeckGUI.cpp
    Created: 14 Jul 2022 1:32:34pm
    Author:  Polly

  ==============================================================================
*/

#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI(DJAudioPlayer* _player,
                 juce::AudioFormatManager & formatManagerToUse,
                 juce::AudioThumbnailCache & cacheToUse) : player(_player),
                                                            waveformDisplay(formatManagerToUse, cacheToUse),
                 loopActive(false)

{
    //load buttons

    
    addAndMakeVisible(playButton);
    addAndMakeVisible(stopButton);
//    addAndMakeVisible(cueButton);
    addAndMakeVisible(loopButton);
//    addAndMakeVisible(prevButton);
//    addAndMakeVisible(nextButton);
    
    addAndMakeVisible(volSlider);
    addAndMakeVisible(speedSlider);
    addAndMakeVisible(posSlider);
    
    
    
    addAndMakeVisible(waveformDisplay);
    
//    sliders settings
    volSlider.setSliderStyle(juce::Slider::SliderStyle::LinearBarVertical);
//    volSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    speedSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
    posSlider.setSliderStyle(juce::Slider::SliderStyle::Rotary);
    posSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 100, 25);
//    set slider colours
    getLookAndFeel().setColour(juce::Slider::thumbColourId, juce::Colours::lightgreen);
    getLookAndFeel().setColour(juce::Slider::rotarySliderFillColourId, juce::Colours::lightgreen);
    
    
//    buttons settings
    
    
//    add listeners
    playButton.addListener(this);
    stopButton.addListener(this);
//    cueButton.addListener(this);
    loopButton.addListener(this);
//    prevButton.addListener(this);
//    nextButton.addListener(this);
    
    volSlider.addListener(this);
    speedSlider.addListener(this);
    posSlider.addListener(this);
    
//    set range for sliders
    volSlider.setRange(0.0, 1.0);
    speedSlider.setRange(0.0, 10.0);
    posSlider.setRange(0.0, 1.0);
    
//    set initial value for vol and speed sliders
    volSlider.setValue(0.1);
    speedSlider.setValue(1.0);
    
    startTimer(100);

}

DeckGUI::~DeckGUI()
{
    stopTimer();
}

void DeckGUI::paint (juce::Graphics& g)
{
        g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
    
}

void DeckGUI::resized()
{
    double rowH = getHeight() / 20;
    int border = 20;
    double sliderWidth = (getWidth() - border * 2) / 2;
    double sliderHeight = rowH * 5;
    int sliderPosY = rowH * 15;

    waveformDisplay.setBounds(border/2, border*2, getWidth() - border*5 + 15, rowH * 6);
    volSlider.setBounds(getWidth() - border*4 + 10, border*2, 70, sliderHeight + 17);
    
    speedSlider.setBounds(border, sliderPosY, sliderWidth, sliderHeight);
    posSlider.setBounds(sliderWidth + border, sliderPosY, sliderWidth, sliderHeight);
    
//    cueButton.setBounds(sliderWidth * 2 + border * 4, rowH * 8 + border, sliderWidth/2, sliderHeight/2);
    playButton.setBounds(border, rowH * 8 + sliderHeight/2 + border, sliderWidth/2, sliderHeight/2);
    stopButton.setBounds(sliderWidth/2 + border * 2, rowH * 8 + sliderHeight/2 + border, sliderWidth/2, sliderHeight/2);
    
    loopButton.setBounds(sliderWidth + border + border * 2, rowH * 8 + sliderHeight/2 + border, sliderWidth/2, sliderHeight/2);
//    prevButton.setBounds(sliderWidth * 3 + border * 2, rowH * 8 + sliderHeight/2 + border, sliderWidth/2, sliderHeight/2);
//    nextButton.setBounds(sliderWidth * 3 + border * 2, rowH * 8 + sliderHeight + border, sliderWidth/2, sliderHeight/2);


    

}


void DeckGUI::buttonClicked(juce::Button* button) {
    if (button == &playButton) {
        player->start();
    }
    if (button == &stopButton) {
        player->stop();
    }
    
    if (button == &loopButton) {
        if (loopActive == false) {
            loopActive = true;
            player->loop(true);
            loopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::lightsalmon); //change the button color to identify that the loop is on
        } else {
            loopActive = false;
            player->loop(false);
            loopButton.setColour(juce::TextButton::buttonColourId, juce::Colours::grey);
        }
    }
}


void DeckGUI::sliderValueChanged(juce::Slider *slider) {
    if (slider == &volSlider) {
        player->setGain(slider->getValue());
    }
    if (slider == &speedSlider) {
        player->setSpeed(slider->getValue());
    }

    if (slider == &posSlider) {
        player->setPositionRelative(slider->getValue());
    }
}


bool DeckGUI::isInterestedInFileDrag (const juce::StringArray &files) {
    std::cout << "DeckGUI::isInterestedInFileDrag" << std::endl;
    return true;
}

void DeckGUI::filesDropped (const juce::StringArray &files, int x, int y) {
    for (juce::String filename : files) {
        std::cout << "DeckGUI::filesDropped " << filename << std::endl;
        player->loadURL(juce::URL{juce::File{filename}});
        return;
    }
}

void DeckGUI::timerCallback() {
    waveformDisplay.setPositionRelative(player->getPositionRelative());
    waveformDisplay.setTimeInSeconds(player->getTimeInSeconds());
}

void DeckGUI::loadFromPlaylist() {
    //open playlist.txt which contains tracks which are going to be uploaded to deckGUI
    std::ifstream file("/Users/polly/Desktop/test/playlist.txt");
    std::string str;
    std::getline(file, str);
    std::string URL = "file:///" + str; //convert URL into file
    juce::URL audioURL{ URL };
    player->loadURL(audioURL); //send URL to player
    waveformDisplay.loadURL(audioURL);
}
