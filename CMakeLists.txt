cmake_minimum_required(VERSION 3.15)
project(DSAProject3)

# Enable C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add the executable
add_executable(PianoSongRank
    src/main.cpp
    src/Parsing.cpp
    src/MidiCollection.cpp
    src/Heap.cpp
)

# Add SFML submodule and include directories
add_subdirectory(external/SFML)
target_include_directories(PianoSongRank PRIVATE external/SFML/include)

# Add Midifile submodule and include directories
add_subdirectory(external/midifile)
target_include_directories(PianoSongRank PRIVATE external/midifile/include)

# Link libraries
target_link_libraries(PianoSongRank PRIVATE
    sfml-graphics       # SFML components
    sfml-window
    sfml-system
    sfml-audio
    midifile            # Midifile library
)
