//
// Created by dtpreda on 23/07/22.
//

#include <gtest/gtest.h>
#include "parser/Terminal.h"
#include "parser/NonTerminal.h"

TEST(SymbolClass, SymbolCreation) {
    ASSERT_NO_FATAL_FAILURE(Symbol("testName"));
}

TEST(SymbolClass, SymbolName) {
    Symbol testSymbol("testSymbol");
    ASSERT_EQ("testSymbol", testSymbol.getName());
}

TEST(SymbolClass, SymbolComparison) {
    Symbol testSymbol1("testSymbol1");
    Symbol testSymbol2("testSymbol2");

    ASSERT_LT(testSymbol1, testSymbol2);
}

TEST(TerminalClass, TerminalCreation) {
    ASSERT_NO_FATAL_FAILURE(Terminal("testName", "testExpression"));
}

TEST(TerminalClass, TerminalName) {
    Terminal testTerminal("testTerminal", "");
    ASSERT_EQ("testTerminal", testTerminal.getName());
}

TEST(TerminalClass, TerminalExpression) {
    Terminal testTerminal("", "(.|\n)*abc(.|\n)*");
    std::string testString("the abc is fun");

    std::regex terminalRegex = testTerminal.getRegex();

    ASSERT_TRUE(std::regex_match(testString, terminalRegex));
}

TEST(NonTerminalClass, NonTerminalCreation) {
    ASSERT_NO_FATAL_FAILURE(NonTerminal("testNonTerminal"));
}

TEST(NonTerminalClass, NonTerminalName) {
    NonTerminal testNonTerminal("testNonTerminal");

    ASSERT_EQ("testNonTerminal", testNonTerminal.getName());
}

TEST(NonTerminalClass, NonTerminalAddRule) {
    NonTerminal testNonTerminal("testNonTerminal");

    Terminal testTerminal1("testTerminal1", "testTerminal1");
    Terminal testTerminal3("testTerminal2", "testTerminal2");
    Terminal testTerminal4("testTerminal3", "testTerminal3");
    Terminal testTerminal2("testTerminal4", "testTerminal4");

    std::vector<Symbol> testRuleExpansion;
    testRuleExpansion.push_back(testTerminal2);
    testRuleExpansion.push_back(testTerminal3);
    testRuleExpansion.push_back(testTerminal4);

    ASSERT_NO_THROW(testNonTerminal.addRule(testTerminal1, testRuleExpansion));

    ASSERT_THROW(testNonTerminal.addRule(testTerminal1, testRuleExpansion), std::runtime_error);

    ASSERT_THROW(testNonTerminal.addRule(testTerminal1, std::vector<Symbol>()), std::runtime_error);
}

TEST(NonTerminalClass, NonTerminalGetRule) {
    NonTerminal testNonTerminal("testNonTerminal");

    Terminal testTerminal1("testTerminal1", "testTerminal1");
    Terminal testTerminal3("testTerminal2", "testTerminal2");
    Terminal testTerminal4("testTerminal3", "testTerminal3");
    Terminal testTerminal2("testTerminal4", "testTerminal4");

    std::vector<Symbol> testRuleExpansion;
    testRuleExpansion.push_back(testTerminal2);
    testRuleExpansion.push_back(testTerminal3);
    testRuleExpansion.push_back(testTerminal4);

    ASSERT_THROW(testNonTerminal.getRule(testTerminal1), std::runtime_error);

    ASSERT_NO_THROW(testNonTerminal.addRule(testTerminal1, testRuleExpansion));

    std::vector<Symbol> returnedRuleExpansion = testNonTerminal.getRule(testTerminal1);

    ASSERT_EQ(returnedRuleExpansion.size(), 3);
    ASSERT_EQ(returnedRuleExpansion.at(0).getName(), testTerminal2.getName());
    ASSERT_EQ(returnedRuleExpansion.at(1).getName(), testTerminal3.getName());
    ASSERT_EQ(returnedRuleExpansion.at(2).getName(), testTerminal4.getName());
}