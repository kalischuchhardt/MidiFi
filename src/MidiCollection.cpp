#include <iostream>
#include <vector>
#include <string>
#include <MidiFile.h>
#include "Parsing.cpp"
#include <filesystem>
#include <unordered_map>

using namespace std;
using namespace smf;
namespace fs = std::filesystem;

class MidiCollection {
public:
    vector<string> midiFiles;
    vector<Parsing> parsedFiles;
    unordered_map<string, vector<pair<string, int>>> difficultyLevel;

    MidiCollection(const string& dirPath) {
        loadMidiFiles(dirPath);
        processFiles();
    }

    void loadMidiFiles(const string& dirPath) {
        for (const auto& entry : fs::directory_iterator(dirPath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".mid") {
                midiFiles.push_back(entry.path().string());
            }
        }
    }

    void processFiles() {
        for (const auto& filePath : midiFiles) {
            try {
                Parsing parsing(filePath);
                parsedFiles.push_back(parsing);
            } catch (const std::exception& e) {
                cerr << "Error processing file " << filePath << ": " << e.what() << endl;
            }
        }
    }

    // MAPS THE SONGS TO A DIFFICULTY LEVEL BASED ON RANK
    void DifficultyLevel() {
        difficultyLevel.clear();

        for (size_t i = 0; i < parsedFiles.size(); i++) {
            int rank = parsedFiles[i].rank();
            string name = parsedFiles[i].getTrackName();

            if (rank <= 142) {
                difficultyLevel["Easy"].emplace_back(name, rank);
            } else if (rank <= 228) {
                difficultyLevel["Intermediate"].emplace_back(name, rank);
            } else {
                difficultyLevel["Hard"].emplace_back(name, rank);
            }
        }
    }

    // PRINTS SONGS NAMES AND DIFFICULTY SCORES AND DIFFICULTY LEVELS
    void printDifficultyLevels() const {
        vector<string> order = {"Easy", "Intermediate", "Hard"};
        for (const auto& difficulty : order) {
            cout << difficulty << " Songs:" << endl;
            for (const auto& [song, rank] : difficultyLevel.at(difficulty)) {
                cout << "  - " << song << " (Rank: " << rank << ")" << endl;
            }
        }
    }
    int getTempo(const string& songName) {
        for (const auto& parsedFile : parsedFiles) {
            if (parsedFile.getTrackName() == songName) {
                return parsedFile.getTempo();
            }
        }
        return -1;
    }

};
