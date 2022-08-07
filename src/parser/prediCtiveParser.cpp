//
// Created by dtpreda on 27/07/22.
//

#include <memory>
#include <vector>

#include "prediCtiveParser.h"
#include "utils.h"
#include "parser/grammar/Terminal.h"
#include "parser/grammar/NonTerminal.h"

Parser buildPrediCtiveParser() {
    std::shared_ptr<Terminal> TOKENS = std::make_shared<Terminal>(Terminal("TokensTerminal", "TOKENS"));
    std::shared_ptr<Terminal> ID = std::make_shared<Terminal>(Terminal("Identifier", "[[:alpha:]][[:alnum:]_]*"));
    std::shared_ptr<Terminal> SKIP = std::make_shared<Terminal>(Terminal("SkipTerminal", "SKIP"));
    std::shared_ptr<Terminal> QUOTE_EXPRESSION = std::make_shared<Terminal>(Terminal("Quote Expression", "\"(.*?)\""));
    std::shared_ptr<Terminal> COMMA = std::make_shared<Terminal>(Terminal("Comma", ","));
    std::shared_ptr<Terminal> LEFT_CURLY_BRACKET = std::make_shared<Terminal>(Terminal("Left Curly Bracket", "\\{"));
    std::shared_ptr<Terminal> RIGHT_CURLY_BRACKET = std::make_shared<Terminal>(Terminal("Right Curly Bracket", "\\}"));
    std::shared_ptr<Terminal> COLON = std::make_shared<Terminal>(Terminal("Colon", "\\:"));
    std::shared_ptr<Terminal> SEMICOLON = std::make_shared<Terminal>(Terminal("Semicolon", "\\;"));
    std::shared_ptr<Terminal> RIGHT_ARROW = std::make_shared<Terminal>(Terminal("Right Arrow", "->"));
    std::shared_ptr<Terminal> OPEN_BRACKET = std::make_shared<Terminal>(Terminal("Open Bracket", "\\<"));
    std::shared_ptr<Terminal> CLOSE_BRACKET = std::make_shared<Terminal>(Terminal("Close Bracket", "\\>"));
    std::shared_ptr<Terminal> OPEN_PARENTHESES = std::make_shared<Terminal>(Terminal("Open Parentheses", "\\("));
    std::shared_ptr<Terminal> CLOSE_PARENTHESES = std::make_shared<Terminal>(Terminal("Close Parentheses", "\\)"));
    std::shared_ptr<Terminal> ASTERISK = std::make_shared<Terminal>(Terminal("Asterisk", "\\*"));
    std::shared_ptr<Terminal> PLUS = std::make_shared<Terminal>(Terminal("Plus", "\\+"));
    std::shared_ptr<Terminal> LAST = std::make_shared<Terminal>(Terminal("Last Token", "last_token"));

    std::vector<std::shared_ptr<Terminal>> terminals = { TOKENS, SKIP, QUOTE_EXPRESSION, LAST, ASTERISK, PLUS, ID, OPEN_BRACKET,
                                               CLOSE_BRACKET, OPEN_PARENTHESES, CLOSE_PARENTHESES, COMMA, COLON, SEMICOLON,
                                               RIGHT_ARROW, LEFT_CURLY_BRACKET, RIGHT_CURLY_BRACKET, END_OF_INPUT };

    std::shared_ptr<NonTerminal> AnnotationOption = std::make_shared<NonTerminal>(("AnnotationOption"));
    AnnotationOption->addToRule(*QUOTE_EXPRESSION, std::vector<std::shared_ptr<Symbol>>({ QUOTE_EXPRESSION }));
    AnnotationOption->addToRule(*LAST, std::vector<std::shared_ptr<Symbol>>({ LAST }));

    std::shared_ptr<NonTerminal> Annotation = std::make_shared<NonTerminal>(("Annotation"));
    Annotation->addToRule(*ID, std::vector<std::shared_ptr<Symbol>>({}));
    Annotation->addToRule(*LEFT_CURLY_BRACKET, std::vector<std::shared_ptr<Symbol>>({ LEFT_CURLY_BRACKET, QUOTE_EXPRESSION, COLON, AnnotationOption, RIGHT_CURLY_BRACKET }));
    Annotation->addToRule(*SEMICOLON, std::vector<std::shared_ptr<Symbol>>({}));
    Annotation->addToRule(*OPEN_BRACKET, std::vector<std::shared_ptr<Symbol>>({}));
    Annotation->addToRule(*OPEN_PARENTHESES, std::vector<std::shared_ptr<Symbol>>({}));
    Annotation->addToRule(*CLOSE_PARENTHESES, std::vector<std::shared_ptr<Symbol>>({}));

    std::shared_ptr<NonTerminal> Closure = std::make_shared<NonTerminal>(("Closure"));
    Closure->addToRule(*ASTERISK, std::vector<std::shared_ptr<Symbol>>({ ASTERISK }));
    Closure->addToRule(*PLUS, std::vector<std::shared_ptr<Symbol>>({ PLUS }));

    std::shared_ptr<NonTerminal> Rule = std::make_shared<NonTerminal>(("Rule"));

    std::shared_ptr<NonTerminal> RuleBlock = std::make_shared<NonTerminal>(("RuleBlock"));
    RuleBlock->addToRule(*ID, std::vector<std::shared_ptr<Symbol>>({ ID }));
    RuleBlock->addToRule(*OPEN_BRACKET, std::vector<std::shared_ptr<Symbol>>({ OPEN_BRACKET, ID, CLOSE_BRACKET }));
    RuleBlock->addToRule(*OPEN_PARENTHESES, std::vector<std::shared_ptr<Symbol>>({ OPEN_PARENTHESES, Rule, CLOSE_PARENTHESES, Closure }));

    std::shared_ptr<NonTerminal> NextRuleBlock = std::make_shared<NonTerminal>(("NextRuleBlock"));
    NextRuleBlock->addToRule(*ID, std::vector<std::shared_ptr<Symbol>>({ RuleBlock, Annotation, NextRuleBlock }));
    NextRuleBlock->addToRule(*LEFT_CURLY_BRACKET, std::vector<std::shared_ptr<Symbol>>({}));
    NextRuleBlock->addToRule(*SEMICOLON, std::vector<std::shared_ptr<Symbol>>({}));
    NextRuleBlock->addToRule(*OPEN_BRACKET, std::vector<std::shared_ptr<Symbol>>({ RuleBlock, AnnotationOption, NextRuleBlock }));
    NextRuleBlock->addToRule(*OPEN_PARENTHESES, std::vector<std::shared_ptr<Symbol>>({ RuleBlock, Annotation, NextRuleBlock }));
    NextRuleBlock->addToRule(*CLOSE_PARENTHESES, std::vector<std::shared_ptr<Symbol>>({}));

    Rule->addToRule(*ID, std::vector<std::shared_ptr<Symbol>>({ RuleBlock, Annotation, NextRuleBlock }));
    Rule->addToRule(*OPEN_BRACKET, std::vector<std::shared_ptr<Symbol>>({ RuleBlock, Annotation, NextRuleBlock }));
    Rule->addToRule(*OPEN_PARENTHESES, std::vector<std::shared_ptr<Symbol>>({ RuleBlock, Annotation, NextRuleBlock }));

    std::shared_ptr<NonTerminal> Rules = std::make_shared<NonTerminal>(("Rules"));

    std::shared_ptr<NonTerminal> NextRule = std::make_shared<NonTerminal>(("NextRule"));
    NextRule->addToRule(*END_OF_INPUT, std::vector<std::shared_ptr<Symbol>>({}));
    NextRule->addToRule(*ID, std::vector<std::shared_ptr<Symbol>>({ ID, RIGHT_ARROW, Rule, SEMICOLON, NextRule }));

    Rules->addToRule(*ID, std::vector<std::shared_ptr<Symbol>>({ ID, RIGHT_ARROW, Rule, SEMICOLON, NextRule }));

    std::shared_ptr<NonTerminal> Skip = std::make_shared<NonTerminal>(("Skip"));

    std::shared_ptr<NonTerminal> SkipExpression = std::make_shared<NonTerminal>(("SkipExpression"));
    SkipExpression->addToRule(*COMMA, std::vector<std::shared_ptr<Symbol>>({ COMMA, QUOTE_EXPRESSION, SkipExpression }));
    SkipExpression->addToRule(*RIGHT_CURLY_BRACKET, std::vector<std::shared_ptr<Symbol>>({}));

    Skip->addToRule(*ID, std::vector<std::shared_ptr<Symbol>>({}));
    Skip->addToRule(*SKIP, std::vector<std::shared_ptr<Symbol>>({ SKIP, LEFT_CURLY_BRACKET, QUOTE_EXPRESSION, SkipExpression, RIGHT_CURLY_BRACKET }));

    std::shared_ptr<NonTerminal> Token = std::make_shared<NonTerminal>(("Token"));
    Token->addToRule(*ID, std::vector<std::shared_ptr<Symbol>>({ ID, COLON, QUOTE_EXPRESSION }));

    std::shared_ptr<NonTerminal> Tokens = std::make_shared<NonTerminal>(("Tokens"));

    std::shared_ptr<NonTerminal> NextToken = std::make_shared<NonTerminal>(("NextToken"));
    NextToken->addToRule(*COMMA, std::vector<std::shared_ptr<Symbol>>({ COMMA, Token, NextToken }));
    NextToken->addToRule(*RIGHT_CURLY_BRACKET, std::vector<std::shared_ptr<Symbol>>({}));

    Tokens->addToRule(*TOKENS, std::vector<std::shared_ptr<Symbol>>({ TOKENS, LEFT_CURLY_BRACKET, Token, NextToken, RIGHT_CURLY_BRACKET }));

    std::shared_ptr<NonTerminal> Start = std::make_shared<NonTerminal>(("Start"));
    Start->addToRule(*TOKENS, std::vector<std::shared_ptr<Symbol>>({ Tokens, Skip, Rules }));

    std::shared_ptr<NonTerminal> SStart = std::make_shared<NonTerminal>(("SStart"));
    SStart->addToRule(*TOKENS, std::vector<std::shared_ptr<Symbol>>({ Start, END_OF_INPUT }));

    return {Recognizer(terminals), *SStart};
}