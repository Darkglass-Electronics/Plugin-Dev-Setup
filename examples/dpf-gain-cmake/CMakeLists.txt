# Example plugin for Anagram using DPF
# Copyright (C) 2025 Filipe Coelho <falktx@darkglass.com>
# SPDX-License-Identifier: ISC

# minimum CMake version, DPF requires 3.8 so we match it here
cmake_minimum_required(VERSION 3.8...3.31)

# C and C++ standards to use
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Hide symbols by default
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

# Setup project languages and version
project(DarkglassExampleGainDPF
    LANGUAGES C CXX
    VERSION 0.0.1
)

# Fetch and import DPF during build, as to not store it in this repository
# You will likely want to have it locally as either a copy or git submodule and then use `add_subdirectory`
include(FetchContent)
FetchContent_Declare(DPF
    GIT_REPOSITORY https://github.com/DISTRHO/DPF.git
    GIT_TAG develop
    GIT_SHALLOW TRUE
    UPDATE_DISCONNECTED TRUE
)
FetchContent_MakeAvailable(DPF)

# Setup a DPF plugin the usual way
dpf_add_plugin(DarkglassExampleGainDPF
    TARGETS lv2
    FILES_DSP
        PluginDSP.cpp
)

# Set include directories
target_include_directories(DarkglassExampleGainDPF
    PUBLIC
        .
)
