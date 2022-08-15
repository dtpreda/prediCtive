//
// Created by dtpreda on 04/08/22.
//

#include <gtest/gtest.h>
#include "TestUtils.h"

#include "parser/prediCtiveParser.h"
#include "parser/ast/TokenExtractorVisitor.h"
#include "parser/ast/SkipExtractorVisitor.h"

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
}

TEST_F(prediCtiveParserTest, TokenExtraction) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    Node root = prediCtiveParser.parse(contents);

    TokenExtractorVisitor tev;

    tev.visit(root);

    Node tokens = root.getChild(0).getChild(0);

    ASSERT_EQ(3, tokens.getChildren().size());

    ASSERT_EQ("INT", tokens.getChild(0).getAnnotation("name"));
    ASSERT_EQ("VOID", tokens.getChild(1).getAnnotation("name"));
    ASSERT_EQ("ID", tokens.getChild(2).getAnnotation("name"));

    ASSERT_EQ("int", tokens.getChild(0).getAnnotation("regex"));
    ASSERT_EQ("void", tokens.getChild(1).getAnnotation("regex"));
    ASSERT_EQ("[[:alpha:]][[:alnum:]_]*", tokens.getChild(2).getAnnotation("regex"));
}

TEST_F(prediCtiveParserTest, SkipExpressionExtraction) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    Node root = prediCtiveParser.parse(contents);

    SkipExtractorVisitor sev;

    sev.visit(root);

    Node skipExpressions = root.getChild(0).getChild(1);

    ASSERT_EQ(3, skipExpressions.getChildren().size());

    //error due to naive open function in TestUtils
    //ASSERT_EQ(" ", skipExpressions.getChild(0).getAnnotation("regex"));
    ASSERT_EQ("\\n", skipExpressions.getChild(1).getAnnotation("regex"));
    ASSERT_EQ("\\t", skipExpressions.getChild(2).getAnnotation("regex"));
}
