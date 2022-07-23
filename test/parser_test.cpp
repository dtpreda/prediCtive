//
// Created by dtpreda on 23/07/22.
//

#include <gtest/gtest.h>
#include "parser/Terminal.h"

TEST(SymbolClass, SymbolCreation) {
    EXPECT_NO_FATAL_FAILURE(Symbol("testName"));
}

TEST(SymbolClass, SymbolName) {
    Symbol testSymbol("testSymbol");
    EXPECT_EQ("testSymbol", testSymbol.getName());
}

TEST(TerminalClass, TerminalCreation) {
    EXPECT_NO_FATAL_FAILURE(Terminal("testName", "testExpression"));
}

TEST(TerminalClass, TerminalName) {
    Terminal testTerminal("testTerminal", "");
    EXPECT_EQ("testTerminal", testTerminal.getName());
}

TEST(TerminalClass, TerminalExpression) {
    Terminal testTerminal("", "(.|\n)*abc(.|\n)*");
    std::string testString("the abc is fun");

    std::regex terminalRegex = testTerminal.getRegex();

    EXPECT_TRUE(std::regex_match(testString, terminalRegex));
}