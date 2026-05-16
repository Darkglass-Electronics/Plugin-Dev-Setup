# minimum CMake version
cmake_minimum_required(VERSION 3.8...3.31)

# C and C++ standards to use
set(CMAKE_C_STANDARD 11)

# Hide symbols by default
set(CMAKE_C_VISIBILITY_PRESET hidden)

project(lv2-anagram-probe C)

find_package(PkgConfig REQUIRED)
pkg_check_modules(LILV REQUIRED lilv-0)

add_executable(lv2-anagram-probe main.c)

target_include_directories(lv2-anagram-probe PRIVATE ${LILV_INCLUDE_DIRS})
target_link_libraries(lv2-anagram-probe PRIVATE ${LILV_LIBRARIES})
