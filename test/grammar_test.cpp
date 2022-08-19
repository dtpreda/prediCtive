//
// Created by dtpreda on 04/08/22->
//

#include <gtest/gtest.h>
#include "TestUtils.h"

#include "parser/prediCtiveParser.h"
#include "parser/ast/TokenExtractorVisitor.h"
#include "parser/ast/SkipExtractorVisitor.h"
#include "parser/ast/RuleExtractorVisitor.h"
#include "parser/ast/RuleSimplifierVisitor.h"
#include "parser/ast/ClosureSimplifierVisitor.h"

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

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    TokenExtractorVisitor tev;

    tev.visit(root);

    std::shared_ptr<Node> tokens = root->getChild(0)->getChild(0);

    ASSERT_EQ(3, tokens->getChildren().size());

    ASSERT_EQ("INT", tokens->getChild(0)->getAnnotation("name"));
    ASSERT_EQ("VOID", tokens->getChild(1)->getAnnotation("name"));
    ASSERT_EQ("ID", tokens->getChild(2)->getAnnotation("name"));

    ASSERT_EQ("int", tokens->getChild(0)->getAnnotation("regex"));
    ASSERT_EQ("void", tokens->getChild(1)->getAnnotation("regex"));
    ASSERT_EQ("[[:alpha:]][[:alnum:]_]*", tokens->getChild(2)->getAnnotation("regex"));
}

TEST_F(prediCtiveParserTest, SkipExpressionExtraction) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    SkipExtractorVisitor sev;

    sev.visit(root);

    std::shared_ptr<Node> skipExpressions = root->getChild(0)->getChild(1);

    ASSERT_EQ(3, skipExpressions->getChildren().size());

    //error due to naive open function in TestUtils
    //ASSERT_EQ(" ", skipExpressions->getChild(0)->getAnnotation("regex"));
    ASSERT_EQ("\\n", skipExpressions->getChild(1)->getAnnotation("regex"));
    ASSERT_EQ("\\t", skipExpressions->getChild(2)->getAnnotation("regex"));
}

TEST_F(prediCtiveParserTest, RuleExtraction) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    RuleExtractorVisitor rev;

    rev.visit(root);

    std::shared_ptr<Node> rules = root->getChild(0)->getChild(2);

    ASSERT_EQ(4, rules->getChildren().size());

    ASSERT_EQ("Start", rules->getChild(0)->getAnnotation("name"));
    ASSERT_EQ("First", rules->getChild(1)->getAnnotation("name"));
    ASSERT_EQ("First", rules->getChild(2)->getAnnotation("name"));
    ASSERT_EQ("Second", rules->getChild(3)->getAnnotation("name"));
}

TEST_F(prediCtiveParserTest, RuleSimplification) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    RuleExtractorVisitor rev;
    rev.visit(root);

    RuleSimplifierVisitor rsv;
    rsv.visit(root);

    std::shared_ptr<Node> rules = root->getChild(0)->getChild(2);

    ASSERT_EQ(4, rules->getChildren().size());

    std::shared_ptr<Node> start = rules->getChild(0);

    ASSERT_EQ(2, start->getChildren().size());

    ASSERT_EQ("Expansion", start->getChild(0)->getName());
    ASSERT_EQ("*", start->getChild(0)->getAnnotation("closure"));
    ASSERT_EQ("first", start->getChild(0)->getAnnotation("test"));
    ASSERT_EQ("NonTerminal", start->getChild(0)->getChild(0)->getName());
    ASSERT_EQ("First", start->getChild(0)->getChild(0)->getAnnotation("name"));

    ASSERT_EQ("NonTerminal", start->getChild(1)->getName());
    ASSERT_EQ("Second", start->getChild(1)->getAnnotation("name"));

    std::shared_ptr<Node> firstFirst = rules->getChild(1);

    ASSERT_EQ(1, firstFirst->getChildren().size());

    ASSERT_EQ("Terminal", firstFirst->getChild(0)->getName());
    ASSERT_EQ("INT", firstFirst->getChild(0)->getAnnotation("name"));
    ASSERT_EQ("last_token", firstFirst->getChild(0)->getAnnotation("hello"));

    std::shared_ptr<Node> secondFirst = rules->getChild(2);

    ASSERT_EQ(1, secondFirst->getChildren().size());

    ASSERT_EQ("Terminal", secondFirst->getChild(0)->getName());
    ASSERT_EQ("VOID", secondFirst->getChild(0)->getAnnotation("name"));

    std::shared_ptr<Node> second = rules->getChild(3);

    ASSERT_EQ(1, second->getChildren().size());

    ASSERT_EQ("Expansion", second->getChild(0)->getName());
    ASSERT_EQ("+", second->getChild(0)->getAnnotation("closure"));
    ASSERT_EQ("Terminal", second->getChild(0)->getChild(0)->getName());
    ASSERT_EQ("ID", second->getChild(0)->getChild(0)->getAnnotation("name"));
}

TEST_F(prediCtiveParserTest, ClosureSimplification) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    RuleExtractorVisitor rev;
    rev.visit(root);

    RuleSimplifierVisitor rsv;
    rsv.visit(root);

    ClosureSimplifierVisitor csv;
    csv.visit(root);

    std::shared_ptr<Node> rules = root->getChild(0)->getChild(2);

    ASSERT_EQ(8, rules->getChildren().size());

    std::shared_ptr<Node> start = rules->getChild(0);

    ASSERT_EQ(2, start->getChildren().size());

    ASSERT_EQ("NonTerminal", start->getChild(0)->getName());
    ASSERT_THROW(start->getChild(0)->getAnnotation("closure"), std::runtime_error);
    ASSERT_EQ("first", start->getChild(0)->getAnnotation("test"));
    ASSERT_EQ("Intermediate_NonTerminal_0", start->getChild(0)->getAnnotation("name"));

    ASSERT_EQ("NonTerminal", start->getChild(1)->getName());
    ASSERT_EQ("Second", start->getChild(1)->getAnnotation("name"));

    std::shared_ptr<Node> firstFirst = rules->getChild(1);

    ASSERT_EQ(1, firstFirst->getChildren().size());

    ASSERT_EQ("Terminal", firstFirst->getChild(0)->getName());
    ASSERT_EQ("INT", firstFirst->getChild(0)->getAnnotation("name"));
    ASSERT_EQ("last_token", firstFirst->getChild(0)->getAnnotation("hello"));

    std::shared_ptr<Node> secondFirst = rules->getChild(2);

    ASSERT_EQ(1, secondFirst->getChildren().size());

    ASSERT_EQ("Terminal", secondFirst->getChild(0)->getName());
    ASSERT_EQ("VOID", secondFirst->getChild(0)->getAnnotation("name"));

    std::shared_ptr<Node> second = rules->getChild(3);

    ASSERT_EQ(2, second->getChildren().size());

    ASSERT_EQ("Terminal", second->getChild(0)->getName());
    ASSERT_EQ("ID", second->getChild(0)->getAnnotation("name"));
    ASSERT_EQ("NonTerminal", second->getChild(1)->getName());
    ASSERT_EQ("Intermediate_NonTerminal_1", second->getChild(1)->getAnnotation("name"));
    ASSERT_THROW(second->getChild(1)->getAnnotation("closure"), std::runtime_error);

    std::shared_ptr<Node> closureRule1 = rules->getChild(4);

    ASSERT_EQ("Intermediate_NonTerminal_0", closureRule1->getAnnotation("name"));

    ASSERT_EQ(2, closureRule1->getChildren().size());

    ASSERT_EQ("NonTerminal", closureRule1->getChild(0)->getName());
    ASSERT_EQ("First", closureRule1->getChild(0)->getAnnotation("name"));
    ASSERT_THROW(closureRule1->getChild(0)->getAnnotation("closure"), std::runtime_error);
    ASSERT_EQ("NonTerminal", closureRule1->getChild(1)->getName());
    ASSERT_EQ("Intermediate_NonTerminal_0", closureRule1->getChild(1)->getAnnotation("name"));

    std::shared_ptr<Node> emptyRule1 = rules->getChild(5);

    ASSERT_EQ("Intermediate_NonTerminal_0", emptyRule1->getAnnotation("name"));

    ASSERT_EQ(0, emptyRule1->getChildren().size());

    std::shared_ptr<Node> closureRule2 = rules->getChild(6);

    ASSERT_EQ("Intermediate_NonTerminal_1", closureRule2->getAnnotation("name"));

    ASSERT_EQ(2, closureRule2->getChildren().size());

    ASSERT_EQ("Terminal", closureRule2->getChild(0)->getName());
    ASSERT_EQ("ID", closureRule2->getChild(0)->getAnnotation("name"));
    ASSERT_THROW(closureRule2->getChild(0)->getAnnotation("closure"), std::runtime_error);
    ASSERT_EQ("NonTerminal", closureRule2->getChild(1)->getName());
    ASSERT_EQ("Intermediate_NonTerminal_1", closureRule2->getChild(1)->getAnnotation("name"));

    std::shared_ptr<Node> emptyRule2 = rules->getChild(7);

    ASSERT_EQ("Intermediate_NonTerminal_1", emptyRule2->getAnnotation("name"));

    ASSERT_EQ(0, emptyRule2->getChildren().size());
}