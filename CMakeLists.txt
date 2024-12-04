cmake_minimum_required(VERSION 3.10)
project(DSAProject3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set SFML paths
set(SFML_ROOT "C:/SFML/SFML-2.5.1") # Change this to your SFML Path
set(SFML_INCLUDE_DIR "${SFML_ROOT}/include")
set(SFML_LIBRARY_DIR "${SFML_ROOT}/lib")
set(SFML_DIR "${SFML_ROOT}/lib/cmake/SFML")

# Find SFML
find_package(SFML 2.5.1 COMPONENTS graphics audio window system REQUIRED)

# Add the executable
add_executable(PianoSongRank
        src/main.cpp
        src/Parsing.cpp
        src/MidiCollection.cpp
        src/Heap.cpp
)

# Add Midifile submodule and include directories
add_subdirectory(external/midifile)
target_include_directories(PianoSongRank PRIVATE
        external/midifile/include
        ${SFML_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(PianoSongRank PRIVATE
        midifile
        sfml-graphics
        sfml-audio
        sfml-window
        sfml-system
)

# Copy SFML DLLs
if(WIN32)
    file(GLOB SFML_DLLS "${SFML_ROOT}/bin/*.dll")
    file(COPY ${SFML_DLLS} DESTINATION ${CMAKE_BINARY_DIR})
endif()
