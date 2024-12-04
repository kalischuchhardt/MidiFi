cmake_minimum_required(VERSION 3.28)
project(MidiDsaProj3)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)

# Set runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Option for shared/static libraries
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

# Include FetchContent for external dependencies
include(FetchContent)

# Fetch SFML
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.2
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
)
FetchContent_MakeAvailable(SFML)

# Fetch Midifile
FetchContent_Declare(
        midifile
        GIT_REPOSITORY https://github.com/craigsapp/midifile.git
        GIT_TAG master
)
FetchContent_MakeAvailable(midifile)

# Add the executable
add_executable(MidiDsaProj3
        src/main.cpp
        src/Heap.cpp
        src/MidiCollection.cpp
        src/Parsing.cpp
)

# Include directories for Midifile
target_include_directories(MidiDsaProj3 PRIVATE ${midifile_SOURCE_DIR}/include)

# Link SFML and Midifile libraries
target_link_libraries(MidiDsaProj3 PRIVATE
        sfml-graphics
        sfml-window
        sfml-system
        sfml-audio
        midifile
)

# Enable compile features
target_compile_features(MidiDsaProj3 PRIVATE cxx_std_17)

# Copy OpenAL DLL on Windows
if(WIN32)
    add_custom_command(
            TARGET MidiDsaProj3
            COMMENT "Copy OPENAL DLL"
            PRE_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll
            $<TARGET_FILE_DIR:MidiDsaProj3>
    )
endif ()
