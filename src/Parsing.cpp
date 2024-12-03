#include <iostream>
#include <MidiFile.h>
#include <MidiEvent.h>
#include <filesystem>

using namespace std;
using namespace smf;
namespace fs = std::filesystem;

class Parsing {
public:
    MidiFile midiFile;
    int trackCount;
    int noteCount;
    int minNote;
    int maxNote;
    int tempo;
    string trackName;

    Parsing(const string& fileName) {
        if (!midiFile.read(fileName)) {
            throw std::runtime_error("Error reading MIDI file: " + fileName);
        }
        extractFeatures(fileName);
    }

private:
    void loadMidiFiles(const string& dirPath) {
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (entry.path().extension() == ".mid") {
                string fileName = entry.path().string();
                if (!midiFile.read(fileName)) {
                    cerr << "Error reading MIDI file: " << fileName << endl;
                } else {
                    try {
                        extractFeatures(fileName);
                    } catch (const std::exception& e) {
                        cerr << "Error extracting features from " << fileName << ": " << e.what() << endl;
                    }
                }
            }
        }
    }

    void extractFeatures(const string& fileName) {
        trackCount = midiFile.getTrackCount();
        noteCount = 0;
        minNote = 128; // HIGHEST MIDI NOTE
        maxNote = -1;  // LOWEST MIDI NOTE
        tempo = 0;
        trackName = extractName(midiFile.getFilename());

        for (int trackIndex = 0; trackIndex < trackCount; trackIndex++) {
            MidiEventList& track = midiFile[trackIndex];
            for (int i = 0; i < track.size(); i++) {
                const MidiEvent& midiEvent = track[i];

                if (midiEvent.isNoteOn() || midiEvent.isNoteOff()) {
                    int note = midiEvent.getKeyNumber();
                    noteCount++;

                    if (note < minNote) {
                        minNote = note;
                    }
                    if (note > maxNote) {
                        maxNote = note;
                    }
                }

                if (midiEvent.isTempo() && tempo == 0) {
                    tempo = midiEvent.getTempoBPM();
                }
            }
        }
    }


    string extractName(const std::string& filePath) {
        //finding last / or \"
        int lastSlash = filePath.find_last_of("/\\");
        string TrackName = filePath.substr(lastSlash + 1);

        //remobing fot
        int lastDot = TrackName.find_last_of('.');
        if (lastDot != std::string::npos) {
            TrackName = TrackName.substr(0, lastDot);
        }
        return TrackName;
    }

    // GETTERS

public:

    int getTrackCount() const {
        return trackCount;
    }

    int getNoteCount() const {
        return noteCount;
    }

    int getTempo() const {
        return tempo;
    }

    int getMinNote() const {
        return minNote;
    }

    int getMaxNote() const {
        return maxNote;
    }

    string getTrackName() const {
        return trackName;
    }

    int rank() const {
        int difficulty = 0;

        difficulty += getTrackCount() * 3;  // MORE TRACKS -> MORE HARD
        difficulty += getNoteCount() / 30;    // MORE NOTES -> MORE HARD
        difficulty += (getMaxNote() - getMinNote()) * 2;  // WIDER NOTE RANGE MAY MAKE IT HARDER
        difficulty += getTempo() / 15; // HIGHER TEMPO MIGHT MAKE IT EASIER

        return difficulty;
    }
};
