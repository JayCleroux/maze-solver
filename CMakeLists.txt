cmake_minimum_required(VERSION 3.17)
project(Assignment_2)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-static")

add_executable(Assignment_2
        src/main.cpp src/MazeSolver.cpp src/MazeSolver.h src/Stack.cpp src/Stack.h)
