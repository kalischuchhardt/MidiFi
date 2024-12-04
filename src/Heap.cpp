#include <vector>
#include <string>
#include "MidiCollection.cpp"

class Heap {
    public:

    struct Song {
        int difficultyScore;
        string name;
        string level;

        Song() = default;

        Song(int score, const string& trackName, const string& difficultyLevel)
            : difficultyScore(score), name(trackName), level(difficultyLevel) {}

        bool operator<(const Song& other) const {
            return difficultyScore < other.difficultyScore;
        }
    };

    private:
    MidiCollection midiCollection;
    vector<Song> heap;

    void heapifyMax(vector<Song>& heap, int heapSize, int index) { // heapifies for max heap
        int largest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        if (left < heapSize && heap[left].difficultyScore > heap[largest].difficultyScore) {
            largest = left;
        }

        if (right < heapSize && heap[right].difficultyScore > heap[largest].difficultyScore) {
            largest = right;
        }

        if (largest != index) {
            std::swap(heap[index], heap[largest]);
            heapifyMax(heap, heapSize, largest); // subtree
        }
    }

    void heapifyMin(vector<Song>& heap, int heapSize, int index) { // heapifies for min heap
        int smallest = index;
        int left = 2 * index + 1;
        int right = 2 * index + 2;

        // if left exists and is smaller than current smallest
        if (left < heapSize && heap[left].difficultyScore < heap[smallest].difficultyScore) {
            smallest = left;
        }

        // if right exists and is smaller than current smallest
        if (right < heapSize && heap[right].difficultyScore < heap[smallest].difficultyScore) {
            smallest = right;
        }

        // if smallest is not current swap and do recursion
        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            heapifyMin(heap, heapSize, smallest); // going down subtree
        }
    }

public:

    Heap(const string& dirPath) : midiCollection(dirPath) {
    }

    void insertIntoHeap() {
        midiCollection.DifficultyLevel(); // makes sure map is populated -- lowkey might not need
        for (auto level : midiCollection.difficultyLevel ) {
            string difficultyLvl = level.first;
            vector<pair<string, int>> songs = level.second;
            Song song;

            for (auto songPair : songs) { // "EASY": {[ "Song", 100 ], ["Other", 120] ... } --- how actual thing looks
                song.difficultyScore = songPair.second; // number score
                song.name = songPair.first;
                song.level = difficultyLvl; // lvl (easy, med, whatever)

                heap.push_back(song);
                // add more info for frontend ??
                // might not need bc access song info by name or smth
            }
        }
    }

    void buildMaxHeap() { // heapifies in a way that makes it max heap
        int heapSize = heap.size();

        for (int i = heapSize / 2 - 1; i >= 0; i--) {
            heapifyMax(heap, heapSize, i);
        }
    }

    void buildMinHeap() { // heapifies in a way that makes it min heap
        int heapSize = heap.size();

        for (int i = heapSize /2 - 1; i >=0; i--) {
            heapifyMin(heap, heapSize, i);
        }
    }

    string getAllSongInfo() { // gets everything in the heap for frontend
        string bigString = "";
        for (auto song : heap) {
            bigString += song.name + " "
                    + to_string(song.difficultyScore) + " "
                    + song.level + ", "; // + to_string(song.tempo) + " " + song.key + ",";
            // for front end parsing, each whole song separated by a comma,
            // each bit of info per song separated by space
        }
        return bigString;
    }

    string getTopItemName() { // gets name about the top element in heap - should change to all top info???
        if (heap.empty()) {
            return "Error: heap empty";
        }
        return heap.front().name;
    }

    vector<Song> getTopNSongs(int n, bool isMax) {
        vector<Song> result;
        vector<Song> tempHeap = heap;

        if (isMax) {
            buildMaxHeap();
        } else {
            buildMinHeap();
        }

        for (int i = 0; i < n && !tempHeap.empty(); ++i) {
            result.push_back(tempHeap[0]);
            swap(tempHeap[0], tempHeap.back());
            tempHeap.pop_back();

            if (isMax) {
                heapifyMax(tempHeap, tempHeap.size(), 0);
            } else {
                heapifyMin(tempHeap, tempHeap.size(), 0);
            }
        }

        return result;
    }

    MidiCollection& getMidiCollection() {
        return midiCollection;
    }

};
//hello
