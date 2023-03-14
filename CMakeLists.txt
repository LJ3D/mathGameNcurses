cmake_minimum_required(VERSION 3.0)

project(mathGameNcurses)

set(CMAKE_CXX_STANDARD 20)

set(SOURCE_FILES main.cpp)

add_executable(mathGameNcurses ${SOURCE_FILES})

target_link_libraries(mathGameNcurses ncurses)