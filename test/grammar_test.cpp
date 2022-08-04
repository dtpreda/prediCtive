//
// Created by dtpreda on 04/08/22.
//

#include <gtest/gtest.h>
#include "TestUtils.h"

#include "parser/prediCtiveParser.h"

class prediCtiveParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        prediCtiveParser = buildPrediCtiveParser();
    }

    Parser prediCtiveParser = Parser(Recognizer(), NonTerminal("NULL"));
};

TEST_F(prediCtiveParserTest, SimpleGrammar) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    Node root = prediCtiveParser.parse(contents);
    ASSERT_EQ(root.getName(), "SStart");

    Node start = root.getChild(0);
    Node eof = root.getChild(1);
    ASSERT_EQ(start.getName(), "Start");
    ASSERT_EQ(eof.getName(), "EOF");

    Node tokens = start.getChild(0);
    ASSERT_EQ(tokens.getName(), "Tokens");

    ASSERT_EQ(tokens.getChild(0).getName(), "Tokens");
    ASSERT_EQ(tokens.getChild(1).getName(), "Left Curly Bracket");
    Node token = tokens.getChild(2);
    ASSERT_EQ(token.getName(), "Token");
    Node nextToken = tokens.getChild(3);
    ASSERT_EQ(nextToken.getName(), "NextToken");
    ASSERT_EQ(tokens.getChild(4).getName(), "Right Curly Bracket");
}