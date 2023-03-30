cmake_minimum_required(VERSION 3.4)
project(cs564_project)

set(CMAKE_CXX_STANDARD 17)

enable_testing()

add_library(
        utilities
        src/utilities/exception.cpp
        src/utilities/exception.hpp
        src/utilities/test.hpp
)

add_library(
        sqlite
        src/dependencies/sqlite/sqlite3.c
        src/dependencies/sqlite/sqlite3.h
)

add_subdirectory(src/projects/2-page-cache)
