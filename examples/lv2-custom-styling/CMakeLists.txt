# Example plugin for Anagram using raw LV2
# Copyright (C) 2025 Filipe Coelho <falktx@darkglass.com>
# SPDX-License-Identifier: ISC

# minimum CMake version
cmake_minimum_required(VERSION 3.8...3.31)

# C and C++ standards to use
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)

# Hide symbols by default
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Setup project languages and version
project(DarkglassExampleStyling
    LANGUAGES C CXX
    VERSION 0.0.1
)

# Fetch and import LV2 and extensions during build, as to not store them in this repository
# You will likely want to have them locally as either a copy or git submodule
include(FetchContent)
FetchContent_Declare(LV2
    GIT_REPOSITORY https://github.com/lv2/lv2.git
    GIT_TAG main
    GIT_SHALLOW TRUE
    UPDATE_DISCONNECTED TRUE
)
FetchContent_MakeAvailable(LV2)

# Setup a shared library as the plugin binary
add_library(DarkglassExampleStyling SHARED)

set_target_properties(DarkglassExampleStyling
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/DarkglassExampleStyling.lv2"
        PREFIX ""
)

target_sources(DarkglassExampleStyling
    PUBLIC
        Styling.c
)

# Set include directories
target_include_directories(DarkglassExampleStyling
    PUBLIC
        ${lv2_SOURCE_DIR}
)

# install ttl data
configure_file("DarkglassExampleStyling.lv2/manifest.ttl.in"
               "${CMAKE_CURRENT_BINARY_DIR}/DarkglassExampleStyling.lv2/manifest.ttl")

configure_file("DarkglassExampleStyling.lv2/plugin.ttl"
               "${CMAKE_CURRENT_BINARY_DIR}/DarkglassExampleStyling.lv2/plugin.ttl")

configure_file("DarkglassExampleStyling.lv2/styling.ttl"
               "${CMAKE_CURRENT_BINARY_DIR}/DarkglassExampleStyling.lv2/styling.ttl")

# TODO install block images in LV2 bundle
#configure_file(anagram-block-off.png ${LV2_BUNDLE}/anagram-block-off.png COPYONLY)
#configure_file(anagram-block-on.png ${LV2_BUNDLE}/anagram-block-on.png COPYONLY)
