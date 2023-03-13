/*
  ==============================================================================

    PlaylistComponent.h
    Created: 10 Aug 2022 5:02:52pm
    Author:  Polly

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
//#include "MainComponent.h"
#include <vector>
#include <string>
#include <fstream>
#include <ostream>
#include <iostream>


//==============================================================================
/*
*/
class PlaylistComponent  : public juce::Component,
                           public juce::TableListBoxModel,
                           public juce::Button::Listener,
                           public juce::TextEditor::Listener
{
public:
    PlaylistComponent();
    ~PlaylistComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    int getNumRows() override; 
    void paintRowBackground (juce::Graphics &,
                             int rowNumber,
                             int width,
                             int height,
                             bool rowIsSelected) override;
    
    void paintCell(juce::Graphics &,
                   int rowNumber,
                   int columnId,
                   int width,
                   int height,
                   bool rowIsSelected) override;
    
    juce::Component * refreshComponentForCell (int rowNumber,
                                         int columnId,
                                         bool isRowSelected,
                                         Component *existingComponentToUpdate) override;
    
    /** implement Button::Listener */
    void buttonClicked (juce::Button* button) override;
    /**Called when the user changes the text in some way.*/
    void textEditorTextChanged (juce::TextEditor & editor) override;
    
    juce::String sendFileToDeckGUI;
    std::string sendFileToDeck;

    
private:
    juce::TableListBox tableComponent;
    
    juce::TextButton loadButton{"LOAD FILE"};
    juce::TextButton deleteButton{"DELETE"};
    juce::TextButton loadToDeck1Button{"LOAD DECK 1"};
    juce::TextButton loadToDeck2Button{"LOAD DECK 2"};
    juce::TextEditor searchPlaylist{"Search..."};
    

    juce::String searchInput;
    juce::Array<juce::File> tracksToAdd;
    juce::String trackName;
    juce::Array<juce::String> trackNames;
    juce::AudioFormatManager formatManager;
    double length;
    juce::Array<double> tracksLength;
    juce::Array<juce::String> filesPath;
    juce::SparseSet<int> selected;
    
    
    /** get filename without  extention or extention without a filename*/
    juce::String getNameOrExtention(juce::String trackName, juce::String which);
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PlaylistComponent)
};
