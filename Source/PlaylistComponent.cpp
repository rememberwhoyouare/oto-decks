/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 10 Aug 2022 5:02:52pm
    Author:  Polly

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    
    tableComponent.getHeader().addColumn("Track", 1, 500);
    tableComponent.getHeader().addColumn("Type", 2, 200);
    tableComponent.getHeader().addColumn("Length", 3, 150);

    
    tableComponent.setModel(this);

    addAndMakeVisible(loadButton);
    addAndMakeVisible(deleteButton);
    addAndMakeVisible(loadToDeck1Button);
    addAndMakeVisible(loadToDeck2Button);
    addAndMakeVisible(searchPlaylist);
    
    addAndMakeVisible(tableComponent);
    
    
//    add listeners
    loadButton.addListener(this);
    deleteButton.addListener(this);
    loadToDeck1Button.addListener(this);
    loadToDeck2Button.addListener(this);
    searchPlaylist.addListener(this);

    
    formatManager.registerBasicFormats();
    tableComponent.setMultipleSelectionEnabled(true); //enable multiple row selection
    
    std::ofstream myfile("/Users/polly/Desktop/test/playlist.txt");
    myfile << sendFileToDeck << std::endl << "0" << std::endl;
    myfile.close();
    
    //create a txt file which stores current playlist audio files URLs
    std::ifstream file("/Users/polly/Desktop/test/PlaylistHistory.txt");
    if (file.fail()) {
        std::cerr << "Error loading PlaylistHistory.txt at the start of the program" << std::endl;
    } else {
        std::cout << "Succesfully loaded PlaylistHistory.txt at the start of the program" << std::endl;
    }
    std::string str;
    std::vector<std::string> lines;
    while (std::getline(file, str)) {
        lines.push_back(str);
    }
    



    
    //if PlaylistHistory.txt contains URLs, add them to trackNames array
    for (int i = 0; i < lines.size(); ++i) {
        filesPath.add(lines[i]);
    }
    
    //if filesPath contains any files, add them to the tracksToadd array
    if (filesPath.size() != 0) {
        DBG("contains files");
        for (int i = 0; i < filesPath.size(); ++i) {
            tracksToAdd.add(juce::File {filesPath[i]});
        }
        for (int i = 0; i < tracksToAdd.size(); ++i) {
            //extract a track name and add it to the array
            trackName = juce::URL::removeEscapeChars(juce::URL{ tracksToAdd[i] }.getFileName());
            trackNames.add(trackName);
            //create reader and calculate length of every track
            juce::AudioFormatReader* reader = formatManager.createReaderFor(tracksToAdd[i]);
            if (reader != nullptr) { //good file
                length = reader->lengthInSamples / reader->sampleRate; //calculalte length of the track by dividing the length in samples by the sample rate
                delete reader;
            }
            tracksLength.add(length); // add length to the array
        }
        tableComponent.updateContent();
    }
    
}

PlaylistComponent::~PlaylistComponent()
{
}

void PlaylistComponent::paint (juce::Graphics& g)
{

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background
//    g.fillAll (juce::Colours::lightskyblue);
//    g.setColour (juce::Colours::lightskyblue);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    loadButton.setBounds(0, 0, getWidth() * 0.2, getHeight()/20);
    deleteButton.setBounds(getWidth() * 0.2, 0, getWidth()/5, getHeight()/20);
    loadToDeck1Button.setBounds(getWidth() * 0.4, 0, getWidth()/5, getHeight()/20);
    loadToDeck2Button.setBounds(getWidth() * 0.6, 0, getWidth()/5, getHeight()/20);
    searchPlaylist.setBounds(getWidth() * 0.8, 0, getWidth()/5, getHeight()/20);
    tableComponent.setBounds(0, getHeight()/20, getWidth(), getHeight());
    
}


int PlaylistComponent::getNumRows() {
    return trackNames.size();
}

void PlaylistComponent::paintRowBackground (juce::Graphics & g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected) {
    if (rowIsSelected) {
        g.fillAll(juce::Colours::orange);
    } else {
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(juce::Graphics & g,
                                  int rowNumber,
                                  int columnId,
                                  int width,
                                  int height,
                                  bool rowIsSelected) {
    //display name of the audio file
    if (columnId == 1) {
        g.drawText(getNameOrExtention(trackNames[rowNumber], "name"),
                   6,
                   0,
                   width - 4,
                   height,
                   juce::Justification::centredLeft,
                   true);
    }
    //display the type of the audio file
    if (columnId == 2) {
        g.drawText(getNameOrExtention(trackNames[rowNumber], "ext"),
                   6,
                   0,
                   width - 4,
                   height,
                   juce::Justification::centredLeft,
                   true);
    }
    
    if (columnId == 3) {
        //display duration of the audio file
        std::string minsStr;
        std::string secsStr;
        std::string hoursStr;
        //calculate seconds minutes and hours
        int seconds = tracksLength[rowNumber];
        int mins = floor(seconds / 60);
        int secs = seconds % 59;
        int hours = floor(mins / 60);
        //convert integers to strings
        if (mins < 10) {
            minsStr = "0" + std::to_string(mins);
        } else if (mins >= 10) {
            minsStr = std::to_string(mins);
        }
        if (secs < 10) {
            secsStr = "0" + std::to_string(secs);
        } else if (secs >= 10) {
            secsStr = std::to_string(secs);
        }
        if (hours < 10) {
            hoursStr = "0" + std::to_string(hours);
        } else if (hours >= 10) {
            hoursStr = std::to_string(hours);
        }
        
        //draw text
        g.drawText(hoursStr + ":" + minsStr + ":" + secsStr,
                   6,
                   0,
                   width - 4,
                   height,
                   juce::Justification::centredLeft,
                   true);
    }
}


juce::Component* PlaylistComponent::refreshComponentForCell (int rowNumber,
                                     int columnId,
                                     bool isRowSelected,
                                     Component *existingComponentToUpdate) {

    return existingComponentToUpdate;
}

void PlaylistComponent::buttonClicked(juce::Button* button) {
//    load multiple files to the playlist
    if (button == &loadButton) {
        juce::FileChooser chooser{"Select files..."};
        
        if(chooser.browseForMultipleFilesToOpen()) {
            tracksToAdd = chooser.getResults();
            for (int i = 0; i < tracksToAdd.size(); ++i) {
                //get the file name and add it to the tracknames array
                trackName = juce::URL::removeEscapeChars(juce::URL{ tracksToAdd[i] }.getFileName());
                trackNames.add(trackName);
                juce::AudioFormatReader* reader = formatManager.createReaderFor(tracksToAdd[i]);
                if (reader != nullptr) { //good file
                    length = reader->lengthInSamples / reader->sampleRate; //calculalte length of the track by dividing the length in samples by the sample rate
                    delete reader;
                } else {
                    std::cout << "something went wrong!!!" << std::endl;
                }
                tracksLength.add(length); // add length to the array
                filesPath.add(tracksToAdd[i].getFullPathName().toStdString()); //add file path to the filesPath vector
            }
            
            //open history txt file and write added file paths into it, close the file afterwards
            
            std::ofstream history("/Users/polly/Desktop/test/PlaylistHistory.txt");
            if (history.fail()) {
                std::cerr << "Error loading PlaylistHistory.txt while uploading data to the playlist" << std::endl;
            } else {
                std::cout << "Succesfully loaded PlaylistHistory.txt while uploading data to the playlist" << std::endl;
            }
            
            for (int i = 0; i < filesPath.size(); ++i) {
                history << filesPath[i] << std::endl;
            }
            history.close();
        
            tableComponent.updateContent();
        }
    }
    
    //get the selected row
    selected = tableComponent.getSelectedRows();
    
    if (button == &deleteButton) {

        
        for (int i = 0; i < selected.size(); ++i) {
            trackNames.remove(selected[i]); //remove tracks from the trackNames array
            tracksLength.remove(selected[i]); //remove track length from the array
            filesPath.remove(selected[i]); //removes deleted filed from filesPath array
            tableComponent.deselectAllRows(); //deselect rows after removing them
        }
        //Open history txt file to update the current track list
        std::ofstream history("/Users/polly/Desktop/test/PlaylitsHistory.txt");
        for (int i = 0; i < filesPath.size(); ++i) {
            history << filesPath[i] << std::endl;
        }
        history.close();
        tableComponent.updateContent();
    }
    
    if (button == &loadToDeck1Button) {
        for (int i = 0; i < tracksToAdd.size(); ++i) {
            juce::AudioFormatReader* reader = formatManager.createReaderFor(tracksToAdd[i]);
            for (int j = 0; j < selected.size(); ++j) {
                //check which files are selected, convert them to strings and assign to sendFileToDeck string
                if (i == selected[j]) {
                    sendFileToDeckGUI = tracksToAdd[i].getFullPathName();
                    sendFileToDeck = sendFileToDeckGUI.toStdString(); //convert from juce::String to std::string 
                    std::replace(sendFileToDeck.begin(), sendFileToDeck.end(), '\\', '/');
                    //open the playlist.txt file and write paths to tracks which are being sent to DeckGUI
                    std::ofstream myfile("/Users/polly/Desktop/test/playlist.txt");
                    myfile << sendFileToDeck << std::endl << "1" << std::endl;
                    myfile.close();
                    
                    tableComponent.deselectAllRows(); //deselect rows after sending track to deck
                }
            }
        }
    }
    
    if (button == &loadToDeck2Button) {
        //check which files are selected, convert them to strings and assign to sendFileToDeck string
        for (int i = 0; i < tracksToAdd.size(); ++i) {
            juce::AudioFormatReader* reader = formatManager.createReaderFor(tracksToAdd[i]);
            for (int j = 0; j < selected.size(); ++j) {
                if (i == selected[j]) {
                    sendFileToDeckGUI = tracksToAdd[i].getFullPathName(); //convert from juce::String to std::string
                    sendFileToDeck = sendFileToDeckGUI.toStdString();
                    std::replace(sendFileToDeck.begin(), sendFileToDeck.end(), '\\', '/');
                    DBG(sendFileToDeck);
                    //open the playlist.txt file and write paths to tracks which are being sent to DeckGUI
                    std::ofstream myfile("/Users/polly/Desktop/test/playlist.txt");
                    myfile << sendFileToDeck << std::endl << "2" << std::endl;
                    myfile.close();
                    
                    tableComponent.deselectAllRows();  //deselect rows after sending track to deck
                }
            }
        }
    }
}


juce::String PlaylistComponent::getNameOrExtention(juce::String trackName, juce::String which) {
    char ch = '.';
    //get the position of the charachter ch in the string name
    int pos = trackName.indexOfChar(ch);
    
    //return name of the audio track
    if (which == "name") {
        which = trackName.substring(0, pos);
    }
    //return extention of the audio track
    else if (which == "ext") {
        which = trackName.substring(pos + 1, trackName.length());
    }
    return which;
}

void PlaylistComponent::textEditorTextChanged (juce::TextEditor & editor) {
    
    searchInput = searchPlaylist.getText(); //get the input text
    
    //if our playlist contains search input, the correct row will be selected
    for (int i = 0; i < trackNames.size(); ++i) {
        if (trackNames[i].containsIgnoreCase(searchInput) && searchInput != "") {
            tableComponent.selectRow(i);
        }
        if (searchInput == "") {
            tableComponent.deselectAllRows();
        }
        
    }
}
