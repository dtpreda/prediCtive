//
// Created by dtpreda on 04/08/22->
//

#include <gtest/gtest.h>
#include "TestUtils.h"

#include "parser/prediCtiveParser.h"
#include "parser/utils.h"
#include "parser/visitors/astConversion/TokenExtractorVisitor.h"
#include "parser/visitors/astConversion/SkipExtractorVisitor.h"
#include "parser/visitors/astConversion/RuleExtractorVisitor.h"
#include "parser/visitors/astConversion/RuleSimplifierVisitor.h"
#include "parser/visitors/astConversion/ClosureSimplifierVisitor.h"
#include "parser/visitors/semantic/SemanticCheckVisitor.h"
#include "parser/visitors/semantic/KeywordVerifierVisitor.h"

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

TEST_F(prediCtiveParserTest, KeywordVerification) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    KeywordVerifierVisitor kvv;

    ASSERT_NO_THROW(kvv.visit(root));
}

TEST_F(prediCtiveParserTest, TokenExtraction) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    TokenExtractorVisitor tev;

    tev.visit(root);

    std::shared_ptr<Node> tokens = root->getChild(0)->getChild(0);

    ASSERT_EQ(3, tokens->getChildren().size());

    ASSERT_EQ("INT", tokens->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("VOID", tokens->getChild(1)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("ID", tokens->getChild(2)->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ("int", tokens->getChild(0)->getAnnotation(REGEX_LITERAL));
    ASSERT_EQ("void", tokens->getChild(1)->getAnnotation(REGEX_LITERAL));
    ASSERT_EQ("[[:alpha:]][[:alnum:]_]*", tokens->getChild(2)->getAnnotation(REGEX_LITERAL));
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
    //ASSERT_EQ(" ", skipExpressions->getChild(0)->getAnnotation(REGEX_LITERAL));
    ASSERT_EQ("\\n", skipExpressions->getChild(1)->getAnnotation(REGEX_LITERAL));
    ASSERT_EQ("\\t", skipExpressions->getChild(2)->getAnnotation(REGEX_LITERAL));
}

TEST_F(prediCtiveParserTest, RuleExtraction) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    RuleExtractorVisitor rev;

    rev.visit(root);

    std::shared_ptr<Node> rules = root->getChild(0)->getChild(2);

    ASSERT_EQ(4, rules->getChildren().size());

    ASSERT_EQ("Start", rules->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("First", rules->getChild(1)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("First", rules->getChild(2)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("Second", rules->getChild(3)->getAnnotation(SYMBOL_NAME));
}

TEST_F(prediCtiveParserTest, RuleSimplification) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    RuleExtractorVisitor rev;
    ASSERT_NO_THROW(rev.visit(root));

    RuleSimplifierVisitor rsv;
    ASSERT_NO_THROW(rsv.visit(root));

    std::shared_ptr<Node> rules = root->getChild(0)->getChild(2);

    ASSERT_EQ(4, rules->getChildren().size());

    std::shared_ptr<Node> start = rules->getChild(0);

    ASSERT_EQ(2, start->getChildren().size());

    ASSERT_EQ("Expansion", start->getChild(0)->getName());
    ASSERT_EQ("*", start->getChild(0)->getAnnotation(CLOSURE));
    ASSERT_EQ("first", start->getChild(0)->getAnnotation("test"));
    ASSERT_EQ("NonTerminal", start->getChild(0)->getChild(0)->getName());
    ASSERT_EQ("First", start->getChild(0)->getChild(0)->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ("NonTerminal", start->getChild(1)->getName());
    ASSERT_EQ("Second", start->getChild(1)->getAnnotation(SYMBOL_NAME));

    std::shared_ptr<Node> firstFirst = rules->getChild(1);

    ASSERT_EQ(1, firstFirst->getChildren().size());

    ASSERT_EQ("Terminal", firstFirst->getChild(0)->getName());
    ASSERT_EQ("INT", firstFirst->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("last_token", firstFirst->getChild(0)->getAnnotation("hello"));

    std::shared_ptr<Node> secondFirst = rules->getChild(2);

    ASSERT_EQ(1, secondFirst->getChildren().size());

    ASSERT_EQ("Terminal", secondFirst->getChild(0)->getName());
    ASSERT_EQ("VOID", secondFirst->getChild(0)->getAnnotation(SYMBOL_NAME));

    std::shared_ptr<Node> second = rules->getChild(3);

    ASSERT_EQ(1, second->getChildren().size());

    ASSERT_EQ("Expansion", second->getChild(0)->getName());
    ASSERT_EQ("+", second->getChild(0)->getAnnotation(CLOSURE));
    ASSERT_EQ("Terminal", second->getChild(0)->getChild(0)->getName());
    ASSERT_EQ("ID", second->getChild(0)->getChild(0)->getAnnotation(SYMBOL_NAME));
}

TEST_F(prediCtiveParserTest, ClosureSimplification) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    RuleExtractorVisitor rev;
    ASSERT_NO_THROW(rev.visit(root));

    RuleSimplifierVisitor rsv;
    ASSERT_NO_THROW(rsv.visit(root));

    ClosureSimplifierVisitor csv;
    ASSERT_NO_THROW(csv.visit(root));

    std::shared_ptr<Node> rules = root->getChild(0)->getChild(2);

    ASSERT_EQ(8, rules->getChildren().size());

    std::shared_ptr<Node> start = rules->getChild(0);

    ASSERT_EQ(2, start->getChildren().size());

    ASSERT_EQ("NonTerminal", start->getChild(0)->getName());
    ASSERT_THROW(start->getChild(0)->getAnnotation(CLOSURE), std::runtime_error);
    ASSERT_EQ("first", start->getChild(0)->getAnnotation("test"));
    ASSERT_EQ("Intermediate_NonTerminal_0", start->getChild(0)->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ("NonTerminal", start->getChild(1)->getName());
    ASSERT_EQ("Second", start->getChild(1)->getAnnotation(SYMBOL_NAME));

    std::shared_ptr<Node> firstFirst = rules->getChild(1);

    ASSERT_EQ(1, firstFirst->getChildren().size());

    ASSERT_EQ("Terminal", firstFirst->getChild(0)->getName());
    ASSERT_EQ("INT", firstFirst->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("last_token", firstFirst->getChild(0)->getAnnotation("hello"));

    std::shared_ptr<Node> secondFirst = rules->getChild(2);

    ASSERT_EQ(1, secondFirst->getChildren().size());

    ASSERT_EQ("Terminal", secondFirst->getChild(0)->getName());
    ASSERT_EQ("VOID", secondFirst->getChild(0)->getAnnotation(SYMBOL_NAME));

    std::shared_ptr<Node> second = rules->getChild(3);

    ASSERT_EQ(2, second->getChildren().size());

    ASSERT_EQ("Terminal", second->getChild(0)->getName());
    ASSERT_EQ("ID", second->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("NonTerminal", second->getChild(1)->getName());
    ASSERT_EQ("Intermediate_NonTerminal_1", second->getChild(1)->getAnnotation(SYMBOL_NAME));
    ASSERT_THROW(second->getChild(1)->getAnnotation(CLOSURE), std::runtime_error);

    std::shared_ptr<Node> closureRule1 = rules->getChild(4);

    ASSERT_EQ("Intermediate_NonTerminal_0", closureRule1->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ(2, closureRule1->getChildren().size());

    ASSERT_EQ("NonTerminal", closureRule1->getChild(0)->getName());
    ASSERT_EQ("First", closureRule1->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_THROW(closureRule1->getChild(0)->getAnnotation(CLOSURE), std::runtime_error);
    ASSERT_EQ("NonTerminal", closureRule1->getChild(1)->getName());
    ASSERT_EQ("Intermediate_NonTerminal_0", closureRule1->getChild(1)->getAnnotation(SYMBOL_NAME));

    std::shared_ptr<Node> emptyRule1 = rules->getChild(5);

    ASSERT_EQ("Intermediate_NonTerminal_0", emptyRule1->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ(0, emptyRule1->getChildren().size());

    std::shared_ptr<Node> closureRule2 = rules->getChild(6);

    ASSERT_EQ("Intermediate_NonTerminal_1", closureRule2->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ(2, closureRule2->getChildren().size());

    ASSERT_EQ("Terminal", closureRule2->getChild(0)->getName());
    ASSERT_EQ("ID", closureRule2->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_THROW(closureRule2->getChild(0)->getAnnotation(CLOSURE), std::runtime_error);
    ASSERT_EQ("NonTerminal", closureRule2->getChild(1)->getName());
    ASSERT_EQ("Intermediate_NonTerminal_1", closureRule2->getChild(1)->getAnnotation(SYMBOL_NAME));

    std::shared_ptr<Node> emptyRule2 = rules->getChild(7);

    ASSERT_EQ("Intermediate_NonTerminal_1", emptyRule2->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ(0, emptyRule2->getChildren().size());
}


TEST_F(prediCtiveParserTest, BundleVisiting) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    ASSERT_NO_THROW(convertToAST(root));

    std::shared_ptr<Node> tokens = root->getChild(0)->getChild(0);

    ASSERT_EQ(3, tokens->getChildren().size());

    ASSERT_EQ("INT", tokens->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("VOID", tokens->getChild(1)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("ID", tokens->getChild(2)->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ("int", tokens->getChild(0)->getAnnotation(REGEX_LITERAL));
    ASSERT_EQ("void", tokens->getChild(1)->getAnnotation(REGEX_LITERAL));
    ASSERT_EQ("[[:alpha:]][[:alnum:]_]*", tokens->getChild(2)->getAnnotation(REGEX_LITERAL));

    std::shared_ptr<Node> skipExpressions = root->getChild(0)->getChild(1);

    ASSERT_EQ(3, skipExpressions->getChildren().size());

    //error due to naive open function in TestUtils
    //ASSERT_EQ(" ", skipExpressions->getChild(0)->getAnnotation(REGEX_LITERAL));
    ASSERT_EQ("\\n", skipExpressions->getChild(1)->getAnnotation(REGEX_LITERAL));
    ASSERT_EQ("\\t", skipExpressions->getChild(2)->getAnnotation(REGEX_LITERAL));

    std::shared_ptr<Node> rules = root->getChild(0)->getChild(2);

    ASSERT_EQ(8, rules->getChildren().size());

    std::shared_ptr<Node> start = rules->getChild(0);

    ASSERT_EQ(2, start->getChildren().size());

    ASSERT_EQ("NonTerminal", start->getChild(0)->getName());
    ASSERT_THROW(start->getChild(0)->getAnnotation(CLOSURE), std::runtime_error);
    ASSERT_EQ("first", start->getChild(0)->getAnnotation("test"));
    ASSERT_EQ("Intermediate_NonTerminal_0", start->getChild(0)->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ("NonTerminal", start->getChild(1)->getName());
    ASSERT_EQ("Second", start->getChild(1)->getAnnotation(SYMBOL_NAME));

    std::shared_ptr<Node> firstFirst = rules->getChild(1);

    ASSERT_EQ(1, firstFirst->getChildren().size());

    ASSERT_EQ("Terminal", firstFirst->getChild(0)->getName());
    ASSERT_EQ("INT", firstFirst->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("last_token", firstFirst->getChild(0)->getAnnotation("hello"));

    std::shared_ptr<Node> secondFirst = rules->getChild(2);

    ASSERT_EQ(1, secondFirst->getChildren().size());

    ASSERT_EQ("Terminal", secondFirst->getChild(0)->getName());
    ASSERT_EQ("VOID", secondFirst->getChild(0)->getAnnotation(SYMBOL_NAME));

    std::shared_ptr<Node> second = rules->getChild(3);

    ASSERT_EQ(2, second->getChildren().size());

    ASSERT_EQ("Terminal", second->getChild(0)->getName());
    ASSERT_EQ("ID", second->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_EQ("NonTerminal", second->getChild(1)->getName());
    ASSERT_EQ("Intermediate_NonTerminal_1", second->getChild(1)->getAnnotation(SYMBOL_NAME));
    ASSERT_THROW(second->getChild(1)->getAnnotation(CLOSURE), std::runtime_error);

    std::shared_ptr<Node> closureRule1 = rules->getChild(4);

    ASSERT_EQ("Intermediate_NonTerminal_0", closureRule1->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ(2, closureRule1->getChildren().size());

    ASSERT_EQ("NonTerminal", closureRule1->getChild(0)->getName());
    ASSERT_EQ("First", closureRule1->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_THROW(closureRule1->getChild(0)->getAnnotation(CLOSURE), std::runtime_error);
    ASSERT_EQ("NonTerminal", closureRule1->getChild(1)->getName());
    ASSERT_EQ("Intermediate_NonTerminal_0", closureRule1->getChild(1)->getAnnotation(SYMBOL_NAME));

    std::shared_ptr<Node> emptyRule1 = rules->getChild(5);

    ASSERT_EQ("Intermediate_NonTerminal_0", emptyRule1->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ(0, emptyRule1->getChildren().size());

    std::shared_ptr<Node> closureRule2 = rules->getChild(6);

    ASSERT_EQ("Intermediate_NonTerminal_1", closureRule2->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ(2, closureRule2->getChildren().size());

    ASSERT_EQ("Terminal", closureRule2->getChild(0)->getName());
    ASSERT_EQ("ID", closureRule2->getChild(0)->getAnnotation(SYMBOL_NAME));
    ASSERT_THROW(closureRule2->getChild(0)->getAnnotation(CLOSURE), std::runtime_error);
    ASSERT_EQ("NonTerminal", closureRule2->getChild(1)->getName());
    ASSERT_EQ("Intermediate_NonTerminal_1", closureRule2->getChild(1)->getAnnotation(SYMBOL_NAME));

    std::shared_ptr<Node> emptyRule2 = rules->getChild(7);

    ASSERT_EQ("Intermediate_NonTerminal_1", emptyRule2->getAnnotation(SYMBOL_NAME));

    ASSERT_EQ(0, emptyRule2->getChildren().size());
}

TEST_F(prediCtiveParserTest, SemanticCheck) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    ASSERT_NO_THROW(convertToAST(root));

    SemanticCheckVisitor scv;

    ASSERT_NO_THROW(scv.visit(root));
}

TEST_F(prediCtiveParserTest, ASTPrinting) {
    std::string contents = TestUtils::openPrediCtiveFile("simpleGrammar.cg");

    ASSERT_NO_THROW(prediCtiveParser.parse(contents));

    std::shared_ptr<Node> root = prediCtiveParser.parse(contents);

    ASSERT_NO_THROW(convertToAST(root));

    std::cout << root->print() << std::endl;
}