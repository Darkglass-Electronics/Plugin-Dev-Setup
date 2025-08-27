# Example plugin for Anagram using JUCE
# Copyright (C) 2025 Filipe Coelho <falktx@darkglass.com>
# SPDX-License-Identifier: ISC

# minimum CMake version, JUCE8 requires 3.22 so we match it here
cmake_minimum_required(VERSION 3.22...3.31)

# C and C++ standards to use
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Hide symbols by default
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Setup project languages and version
project(DarkglassExampleJUCE
    LANGUAGES C CXX
    VERSION 0.0.1
)

# FIXME this should not be needed
set(JUCE_ANAGRAM_LV2_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/juce-anagram-lv2-src")

# Import the custom Anagram LV2 Wrapper
# You will likely want to have it locally as either a copy or git submodule and then use `add_subdirectory`
include(FetchContent)
FetchContent_Declare(juce-anagram-lv2
    GIT_REPOSITORY https://github.com/Darkglass-Electronics/juce-anagram-lv2.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    UPDATE_DISCONNECTED TRUE
)
FetchContent_MakeAvailable(juce-anagram-lv2)

# Fetch and import JUCE during build, as to not store it in this repository
# You will likely want to have it locally as either a copy or git submodule and then use `add_subdirectory`
include(FetchContent)
FetchContent_Declare(JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG 8.0.8
    GIT_SHALLOW TRUE
    PATCH_COMMAND patch -p1 -i "${JUCE_ANAGRAM_LV2_DIR}/cross-compilation-patches/v8.0.8.patch"
    UPDATE_DISCONNECTED TRUE
)
FetchContent_MakeAvailable(JUCE)

# Setup a JUCE plugin the usual way
juce_add_plugin(DarkglassExampleJUCE
    COMPANY_NAME "Darkglass Electronics Oy"
    COMPANY_WEBSITE "https://github.com/Darkglass-Electronics/Plugin-Dev-Setup"
    PRODUCT_NAME "Example"
    BUNDLE_ID "com.darkglass.anagram-example-juce"
    COPY_PLUGIN_AFTER_BUILD FALSE
    DESCRIPTION "Example plugin for Anagram using JUCE"
    FORMATS LV2
    IS_MIDI_EFFECT FALSE
    IS_SYNTH FALSE
    LV2_URI "https://github.com/Darkglass-Electronics/Plugin-Dev-Setup#example-juce"
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
)

# Enable the custom Anagram LV2 wrapper
juce_anagram_lv2_setup(DarkglassExampleJUCE)

# Set extra compile definitions to reduce features and thus our binary size too
target_compile_definitions(DarkglassExampleJUCE
    PUBLIC
        # disable audio codecs that we will never use
        JUCE_USE_FLAC=0
        JUCE_USE_OGGVORBIS=0
        # disable other stuff
        JUCE_WEB_BROWSER=0
)

# Use JUCE recommended flags
target_link_libraries(DarkglassExampleJUCE
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

target_sources(DarkglassExampleJUCE
    PRIVATE
        PluginProcessor.cpp
)
