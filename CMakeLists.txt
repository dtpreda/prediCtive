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

set(SOURCE_FILES src/parser/Terminal.cpp src/parser/Terminal.h
        src/parser/Symbol.cpp src/parser/Symbol.h
        src/parser/NonTerminal.cpp src/parser/NonTerminal.h
        src/parser/Recognizer.cpp src/parser/Recognizer.h
        src/parser/prediCtiveParser.cpp src/parser/prediCtiveParser.h
        src/parser/Parser.cpp src/parser/Parser.h
)
add_library(testables ${SOURCE_FILES})

add_executable(parser src/main.cpp ${SOURCE_FILES})
add_subdirectory(test)
