cmake_minimum_required(VERSION 3.16)
project(PianoSongRank)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)

# Include FetchContent module
include(FetchContent)

# Fetch the Midifile library
FetchContent_Declare(
        midifile
        GIT_REPOSITORY https://github.com/craigsapp/midifile.git
        GIT_TAG master
)

# Make sure Midifile is available
FetchContent_MakeAvailable(midifile)

# Fetch the SFML library
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.5.1
)

# Make SFML available
FetchContent_MakeAvailable(SFML)

# Add the executable
add_executable(PianoSongRank
        src/main.cpp
        src/Parsing.cpp
        src/MidiCollection.cpp
        src/Heap.cpp
)

# Include Midifile headers
target_include_directories(PianoSongRank PRIVATE ${midifile_SOURCE_DIR}/include)

# Link libraries
target_link_libraries(PianoSongRank PRIVATE sfml-graphics sfml-window sfml-system midifile)
