cmake_minimum_required(VERSION 3.22)
project(parser)

add_subdirectory(google_test)

set(CMAKE_CXX_STANDARD 14)

add_executable(parser
        src/main.cpp)
