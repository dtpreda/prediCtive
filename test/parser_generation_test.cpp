//
// Created by dtpreda on 02/09/22.
//

#include <gtest/gtest.h>
#include "TestUtils.h"

#include "parser/prediCtiveParser.h"
#include "parser/utils.h"
#include "parser/visitors/grammar/GrammarBuilderVisitor.h"

class parserGenerationTest : public ::testing::Test {
protected:
    void SetUp() override {
        prediCtiveParser = buildPrediCtiveParser();
    }

    Parser prediCtiveParser = Parser(Recognizer(), NonTerminal("NULL"));
};

TEST_F(parserGenerationTest, GrammarBuilder) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    ASSERT_NO_THROW(convertToAST(root));

    GrammarBuilderVisitor gbv;

    ASSERT_NO_THROW(gbv.visit(root));
}

TEST_F(parserGenerationTest, GrammarVerifier) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    ASSERT_NO_THROW(convertToAST(root));

    GrammarBuilderVisitor gbv;

    ASSERT_NO_THROW(gbv.visit(root));

    GrammarVerifier gv = gbv.grammarVerifier;
    gv.computeSets();
}