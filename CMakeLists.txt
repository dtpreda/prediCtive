cmake_minimum_required(VERSION 3.22)
project(parser)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)

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

set(SOURCE_FILES
        src/parser/grammar/Terminal.cpp src/parser/grammar/Terminal.h
        src/parser/grammar/Symbol.cpp src/parser/grammar/Symbol.h
        src/parser/grammar/NonTerminal.cpp src/parser/grammar/NonTerminal.h
        src/parser/grammar/Recognizer.cpp src/parser/grammar/Recognizer.h
        src/parser/prediCtiveParser.cpp src/parser/prediCtiveParser.h
        src/parser/Parser.cpp src/parser/Parser.h
        src/parser/ast/Node.cpp src/parser/ast/Node.h
        src/parser/utils.h src/parser/utils.cpp
        src/parser/ast/Visitor.cpp src/parser/ast/Visitor.h
        src/parser/ast/TokenExtractorVisitor.cpp src/parser/ast/TokenExtractorVisitor.h
)
set(TEST_ASSETS test/TestUtils.cpp test/TestUtils.h)
add_library(testables ${SOURCE_FILES} ${TEST_ASSETS})

add_executable(parser src/main.cpp ${SOURCE_FILES})
add_subdirectory(test)
