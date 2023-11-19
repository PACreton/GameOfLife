cmake_minimum_required(VERSION 3.0)
project(GameOfLife C)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")
find_package(raylib REQUIRED)
link_directories(${CMAKE_SOURCE_DIR})

add_executable(game main.c)

target_link_libraries(game raylib)