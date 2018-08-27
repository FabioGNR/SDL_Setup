cmake_minimum_required(VERSION 3.12)
project(SDL_Setup)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIR})

set (SDL_SETUP_SOURCES
        src/main.cpp
        src/SimpleWindow.cpp
        src/SimpleWindow.h
        )

add_executable(SDL_Setup ${SDL_SETUP_SOURCES})
target_link_libraries(SDL_Setup ${SDL2_LIBRARY})
