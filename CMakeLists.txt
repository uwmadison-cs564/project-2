cmake_minimum_required(VERSION 3.11)
project(sqlite_edu)

set(CMAKE_CXX_STANDARD 17)

add_library(
        utilities
        src/utilities/exceptions.hpp
        src/utilities/exceptions.cpp
)

add_library(
        sqlite
        src/dependencies/sqlite/sqlite3.h
        src/dependencies/sqlite/sqlite3.c
)

add_subdirectory(src/projects/1-buffer-management)
