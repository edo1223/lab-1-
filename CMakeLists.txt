cmake_minimum_required(VERSION 3.22)
project(Lab1v2)

set(CMAKE_CXX_STANDARD 14)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(Lab1v2 main.cpp tinyxml2.cpp utils.h structs.h)
