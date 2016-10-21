cmake_minimum_required(VERSION 3.6)
project(8080Emulator)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES
    8080Core.cpp
    8080Core.h
    main.cpp)

add_executable(8080Emulator ${SOURCE_FILES})