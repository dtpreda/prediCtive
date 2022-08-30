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
        src/parser/node/Node.cpp src/parser/node/Node.h
        src/parser/utils.h src/parser/utils.cpp
        src/parser/visitors/Visitor.cpp src/parser/visitors/Visitor.h
        src/parser/visitors/astConversion/TokenExtractorVisitor.cpp src/parser/visitors/astConversion/TokenExtractorVisitor.h
        src/parser/visitors/astConversion/SkipExtractorVisitor.cpp src/parser/visitors/astConversion/SkipExtractorVisitor.h
        src/parser/visitors/astConversion/RuleExtractorVisitor.cpp src/parser/visitors/astConversion/RuleExtractorVisitor.h
        src/parser/visitors/astConversion/RuleSimplifierVisitor.cpp src/parser/visitors/astConversion/RuleSimplifierVisitor.h
        src/parser/visitors/astConversion/ClosureSimplifierVisitor.cpp src/parser/visitors/astConversion/ClosureSimplifierVisitor.h
        src/parser/visitors/semantic/SemanticCheckVisitor.cpp src/parser/visitors/semantic/SemanticCheckVisitor.h
)
set(TEST_ASSETS test/TestUtils.cpp test/TestUtils.h)
add_library(testables ${SOURCE_FILES} ${TEST_ASSETS})

add_executable(parser src/main.cpp ${SOURCE_FILES})
add_subdirectory(test)
