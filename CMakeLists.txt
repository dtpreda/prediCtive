cmake_minimum_required(VERSION 3.22)
project(parser)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include_directories(src)

set(TESTABLES src/parser/Terminal.cpp src/parser/Terminal.h)
add_library(testables ${TESTABLES})

add_executable(parser src/main.cpp src/parser/Terminal.cpp src/parser/Terminal.h)
add_subdirectory(test)
