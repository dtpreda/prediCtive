//
// Created by dtpreda on 23/07/22.
//

#include <gtest/gtest.h>
#include "parser/Terminal.h"
#include "parser/NonTerminal.h"
#include "parser/Recognizer.h"

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

    std::regex terminalRegex(testTerminal.getRegexExpression());

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

TEST(RecognizerClass, RecognizerRecognizeFirstTerminalFullString) {
    Recognizer testRecognizer;
    Terminal testTerminal("testTerminal", "testRegexExpression");

    ASSERT_NO_FATAL_FAILURE(testRecognizer.addTerminal(testTerminal));

    std::string testString("testRegexExpression");

    Terminal recognized = testRecognizer.recognizeFirstTerminal(testString);

    ASSERT_EQ("testTerminal", recognized.getName());
    ASSERT_EQ("", testString);
}

TEST(RecognizerClass, RecognizerRecognizeFirstTerminalAfterNewline) {
    Recognizer testRecognizer;
    Terminal testTerminal("testTerminal", "testRegexExpression");

    ASSERT_NO_FATAL_FAILURE(testRecognizer.addTerminal(testTerminal));

    std::string testString("\ntestRegexExpression");

    Terminal recognized = testRecognizer.recognizeFirstTerminal(testString);

    ASSERT_EQ(Terminal::NULL_TERMINAL, recognized.getName());
    ASSERT_EQ("\ntestRegexExpression", testString);
}

TEST(RecognizerClass, RecognizerRecognizeFirstTerminalMultiple) {
    Recognizer testRecognizer;
    Terminal testTerminal("testTerminal", "t1");
    Terminal testTerminal2("testTerminal2", ";");
    Terminal testTerminal3("testTerminal3", "t2");

    ASSERT_NO_FATAL_FAILURE(testRecognizer.addTerminal(testTerminal));
    ASSERT_NO_FATAL_FAILURE(testRecognizer.addTerminal(testTerminal2));
    ASSERT_NO_FATAL_FAILURE(testRecognizer.addTerminal(testTerminal3));

    std::string testString("t1;t2");

    Terminal recognized1 = testRecognizer.recognizeFirstTerminal(testString);

    ASSERT_EQ("testTerminal", recognized1.getName());
    ASSERT_EQ(";t2", testString);

    Terminal recognized2 = testRecognizer.recognizeFirstTerminal(testString);

    ASSERT_EQ("testTerminal2", recognized2.getName());
    ASSERT_EQ("t2", testString);

    Terminal recognized3 = testRecognizer.recognizeFirstTerminal(testString);

    ASSERT_EQ("testTerminal3", recognized3.getName());
    ASSERT_EQ("", testString);
}

TEST(RecognizerClass, RecognizerRecognizeFirstTerminalPriority) {
    Recognizer testRecognizer1;
    Recognizer testRecognizer2;
    Terminal testTerminal1("testTerminal1", "t1");
    Terminal testTerminal2("testTerminal2", "t12");

    ASSERT_NO_FATAL_FAILURE(testRecognizer1.addTerminal(testTerminal1));
    ASSERT_NO_FATAL_FAILURE(testRecognizer1.addTerminal(testTerminal1));

    ASSERT_NO_FATAL_FAILURE(testRecognizer2.addTerminal(testTerminal2));
    ASSERT_NO_FATAL_FAILURE(testRecognizer2.addTerminal(testTerminal1));

    std::string testString = "t12";

    Terminal recognized1 = testRecognizer1.recognizeFirstTerminal(testString);

    ASSERT_EQ("testTerminal1", recognized1.getName());
    ASSERT_EQ("2", testString);

    testString = "t12";

    Terminal recognized2 = testRecognizer2.recognizeFirstTerminal(testString);

    ASSERT_EQ("testTerminal2", recognized2.getName());
    ASSERT_EQ("", testString);
}

TEST(RecognizerClass, RecognizerRecognizeFirstTerminalUnknown) {
    Recognizer testRecognizer;
    Terminal testTerminal("testTerminal", "t1");

    std::string testString("t1");

    Terminal recognized = testRecognizer.recognizeFirstTerminal(testString);

    ASSERT_EQ(Terminal::NULL_TERMINAL, recognized.getName());
    ASSERT_EQ("t1", testString);
}
