#include <vector>
#include <string>
#include "MidiCollection.cpp"

class Heap {
    public:

    struct Song {
        int difficultyScore;
        string name;
        string level;
        int tempo;

        Song() = default;

        Song(int score, const string& trackName, const string& difficultyLevel, int tempo)
            : difficultyScore(score), name(trackName), level(difficultyLevel), tempo(tempo) {}

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

    void insertIntoHeap(string state) {
        midiCollection.DifficultyLevel(); // makes sure map is populated -- lowkey might not need
        Song song;
        vector<pair<string, int>> songs = midiCollection.difficultyLevel[state];

        if (state == "Easy") {
            // vector<pair<string, int>> songs = midiCollection.difficultyLevel[state] = {};
            for (auto songPair : songs) {
                song.difficultyScore = songPair.second; // number score
                song.name = songPair.first; // song name
                song.level = state;

                heap.push_back(song);
            }
        } else if (state == "Intermediate") {
            // vector<pair<string, int>> songs = midiCollection.difficultyLevel[state] = {};
            for (auto songPair : songs) {
                song.difficultyScore = songPair.second; // number score
                song.name = songPair.first; // song name
                song.level = state;

                heap.push_back(song);
            }
        } else if (state == "Hard"){
            // vector<pair<string, int>> songs = midiCollection.difficultyLevel[state] = {};
            for (auto songPair : songs) {
                song.difficultyScore = songPair.second; // number score
                song.name = songPair.first; // song name
                song.level = state;

                heap.push_back(song);
            }
        } else {
            cout << "something went wrong in the state thingy" << endl;
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
        vector<Song> copyheap = this->heap;

        while (heap.size() > 0) {
            Song song = heap[0];
            bigString += song.name + "   -   " + "Difficulty Score: "
                    + to_string(song.difficultyScore) + "\n\n";
            std::swap(heap[0], heap[heap.size() - 1]);
            heap.pop_back();

            if ()
        }



        // for (auto song : heap) {
        //     bigString += song.name + "   -   " + "Difficulty Score: "
        //             + to_string(song.difficultyScore) + "\n\n";
        //     // for front end parsing, each whole song separated by a comma,
        //     // each bit of info per song separated by space
        // }
        return bigString;
    }

    string getTopItemName() { // gets name about the top element in heap - should change to all top info???
        if (heap.empty()) {
            return "Error: heap empty";
        }
        return heap.front().name;
    }

    void clearMaxHeap() {
        for (auto song :heap) {
            heap.pop_back();
        };
        heapifyMax();
    }

    void clearMinHeap() {
        for (auto song :heap) {
            heap.pop_back();
        };
        heapifyMin();
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
