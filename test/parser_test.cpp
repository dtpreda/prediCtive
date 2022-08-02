//
// Created by dtpreda on 23/07/22.
//

#include <gtest/gtest.h>
#include "parser/Terminal.h"
#include "parser/NonTerminal.h"
#include "parser/Recognizer.h"
#include "parser/Parser.h"
#include "parser/prediCtiveParser.h"

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

TEST(TerminalClass, TerminalAssignmentOperator) {
    Terminal testTerminal1("testTerminal1", "testTerminal1");
    Terminal testTerminal2("testTerminal2", "testTerminal2");

    testTerminal1 = testTerminal2;

    ASSERT_EQ("testTerminal2", testTerminal1.getName());
    ASSERT_EQ("testTerminal2", testTerminal1.getRegexExpression());
}

TEST(NonTerminalClass, NonTerminalCreation) {
    ASSERT_NO_FATAL_FAILURE(NonTerminal("testNonTerminal"));
}

TEST(NonTerminalClass, NonTerminalName) {
    NonTerminal testNonTerminal("testNonTerminal");

    ASSERT_EQ("testNonTerminal", testNonTerminal.getName());
}

TEST(NonTerminalClass, NonTerminalAddAndGetRule) {
    NonTerminal testNonTerminal("testNonTerminal");

    Terminal testTerminal1("testTerminal1", "testTerminal1");
    Terminal testTerminal2("testTerminal2", "testTerminal2");
    Terminal testTerminal3("testTerminal3", "testTerminal3");
    Terminal testTerminal4("testTerminal4", "testTerminal4");
    NonTerminal testNonTerminal2("testNonTerminal2");

    ASSERT_THROW(testNonTerminal.getRule(testTerminal1), std::runtime_error);

    ASSERT_NO_FATAL_FAILURE(testNonTerminal.addToRule(testTerminal1, testTerminal2));
    ASSERT_NO_FATAL_FAILURE(testNonTerminal.addToRule(testTerminal1, testTerminal3));
    ASSERT_NO_FATAL_FAILURE(testNonTerminal.addToRule(testTerminal1, testTerminal4));
    ASSERT_NO_FATAL_FAILURE(testNonTerminal.addToRule(testTerminal1, testNonTerminal2));

    ASSERT_NO_THROW(testNonTerminal.getRule(testTerminal1));

    std::vector<Symbol*> expansionRule = testNonTerminal.getRule(testTerminal1);

    ASSERT_EQ(4, expansionRule.size());
    ASSERT_EQ("testTerminal2", expansionRule.at(0)->getName());
    ASSERT_EQ("testTerminal3", expansionRule.at(1)->getName());
    ASSERT_EQ("testTerminal4", expansionRule.at(2)->getName());
    ASSERT_EQ("testNonTerminal2", expansionRule.at(3)->getName());
}

TEST(NonTerminalClass, NonTerminalAddAndGetRuleVector) {
    NonTerminal testNonTerminal("testNonTerminal");

    Terminal testTerminal1("testTerminal1", "testTerminal1");
    Terminal testTerminal2("testTerminal2", "testTerminal2");
    Terminal testTerminal3("testTerminal3", "testTerminal3");
    Terminal testTerminal4("testTerminal4", "testTerminal4");
    NonTerminal testNonTerminal2("testNonTerminal2");

    ASSERT_THROW(testNonTerminal.getRule(testTerminal1), std::runtime_error);

    std::vector<Symbol*> expansion = { &testTerminal2, &testTerminal3, &testTerminal4, &testNonTerminal2 };

    ASSERT_NO_FATAL_FAILURE(testNonTerminal.addToRule(testTerminal1, expansion));

    ASSERT_NO_THROW(testNonTerminal.getRule(testTerminal1));

    std::vector<Symbol*> expansionRule = testNonTerminal.getRule(testTerminal1);

    ASSERT_EQ(4, expansionRule.size());
    ASSERT_EQ("testTerminal2", expansionRule.at(0)->getName());
    ASSERT_EQ("testTerminal3", expansionRule.at(1)->getName());
    ASSERT_EQ("testTerminal4", expansionRule.at(2)->getName());
    ASSERT_EQ("testNonTerminal2", expansionRule.at(3)->getName());
}

TEST(RecognizerClass, RecognizerCreation) {
    Terminal testTerminal1("testTerminal1", "t1");
    Terminal testTerminal2("testTerminal2", "t12");

    std::vector<Terminal> testTerminals = { testTerminal1, testTerminal2 };

    Recognizer testRecognizer(testTerminals);

    std::string testString("t12");

    Terminal recognized = testRecognizer.recognizeFirstTerminal(testString);

    ASSERT_EQ("testTerminal1", recognized.getName());
    ASSERT_EQ("2", testString);
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

    ASSERT_THROW(testRecognizer.recognizeFirstTerminal(testString), std::runtime_error);
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

    ASSERT_THROW(testRecognizer.recognizeFirstTerminal(testString), std::runtime_error);
}

TEST(prediCtiveParser, prediCtiveParserRecognizeSimple) {
    std::string testString1("TOKENS");
    std::string testString2("identifier1");
    std::string testString3("last_token");
    std::string testString4("SKIP");

    Terminal recognized1 = prediCtiveRecognizer.recognizeFirstTerminal(testString1);

    ASSERT_EQ("Tokens", recognized1.getName());
    ASSERT_EQ("", testString1);

    Terminal recognized2 = prediCtiveRecognizer.recognizeFirstTerminal(testString2);

    ASSERT_EQ("Identifier", recognized2.getName());
    ASSERT_EQ("", testString2);

    Terminal recognized3 = prediCtiveRecognizer.recognizeFirstTerminal(testString3);

    ASSERT_EQ("Last", recognized3.getName());
    ASSERT_EQ("", testString3);

    Terminal recognized4 = prediCtiveRecognizer.recognizeFirstTerminal(testString4);

    ASSERT_EQ("Skip", recognized4.getName());
    ASSERT_EQ("", testString4);
}

TEST(ParserClass, ParserCreation) {
    ASSERT_NO_FATAL_FAILURE(Parser(Recognizer(), NonTerminal("testSymbol")));
}

TEST(ParserClass, ParserParseSimple) {
    NonTerminal testStartSymbol("Start");
    Terminal testTerminal1("Hello", "Hello");
    Terminal testTerminal2("WhiteSpace", "\\s");
    Terminal testTerminal3("World", "World!");
    Terminal testTerminal4("EOF", "\\$");

    testStartSymbol.addToRule(testTerminal1, testTerminal1);
    testStartSymbol.addToRule(testTerminal1, testTerminal2);
    testStartSymbol.addToRule(testTerminal1, testTerminal3);
    testStartSymbol.addToRule(testTerminal1, testTerminal4);

    Recognizer testRecognizer;
    testRecognizer.addTerminal(testTerminal1);
    testRecognizer.addTerminal(testTerminal2);
    testRecognizer.addTerminal(testTerminal3);
    testRecognizer.addTerminal(testTerminal4);

    Parser testParser(testRecognizer, testStartSymbol);

    std::string testString("Hello World!$");
    ASSERT_NO_THROW(testParser.parse(testString));

    std::string testStringFail("World!$ Hello");
    ASSERT_THROW(testParser.parse(testStringFail), std::runtime_error);
}

TEST(ParserClass, ParserParseWithNonTerminals) {
    NonTerminal testStartSymbol("Start");
    Terminal testTerminal1("Hello", "Hello");
    Terminal testTerminal2("WhiteSpace", "\\s");
    Terminal testTerminal3("World", "World!");
    Terminal testTerminal4("EOF", "\\$");

    NonTerminal testExpansion1("Hello World!");
    NonTerminal testExpansion2("World! Hello");

    testExpansion1.addToRule(testTerminal1, testTerminal1);
    testExpansion1.addToRule(testTerminal1, testTerminal2);
    testExpansion1.addToRule(testTerminal1, testTerminal3);

    testExpansion2.addToRule(testTerminal3, testTerminal3);
    testExpansion2.addToRule(testTerminal3, testTerminal2);
    testExpansion2.addToRule(testTerminal3, testTerminal1);
    testExpansion2.addToRule(testTerminal3, testTerminal4);

    testStartSymbol.addToRule(testTerminal1, testExpansion1);
    testStartSymbol.addToRule(testTerminal1, testExpansion2);

    Recognizer testRecognizer;
    testRecognizer.addTerminal(testTerminal1);
    testRecognizer.addTerminal(testTerminal2);
    testRecognizer.addTerminal(testTerminal3);
    testRecognizer.addTerminal(testTerminal4);

    Parser testParser(testRecognizer, testStartSymbol);

    std::string testString("Hello World!World! Hello$");
    ASSERT_NO_THROW(testParser.parse(testString));

    std::string testStringFail("Hello World!World! Hello$$");
    ASSERT_THROW(testParser.parse(testStringFail), std::runtime_error);
}
