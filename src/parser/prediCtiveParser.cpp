//
// Created by dtpreda on 27/07/22.
//

#include "prediCtiveParser.h"
#include "Terminal.h"

static Terminal TOKENS("Tokens", "TOKENS");
static Terminal ID("Identifier", "[[:alpha:]][[:alnum:]_-]*");
static Terminal SKIP("Skip", "SKIP");
static Terminal QUOTE_EXPRESSION("Quote Expression", "\".*\"");
static Terminal COMMA("Comma", ",");
static Terminal LEFT_CURLY_BRACKET("Left Curly Bracket", "\\{");
static Terminal RIGHT_CURLY_BRACKET("Right Curly Bracket", "\\}");
static Terminal COLON("Colon", "\\:");
static Terminal SEMICOLON("Semicolon", "\\;");
static Terminal RIGHT_ARROW("Right Arrow", "->");
static Terminal OPEN_BRACKET("Open Bracket", "\\<");
static Terminal CLOSE_BRACKET("Close Bracket", "\\>");
static Terminal OPEN_PARENTHESES("Open Parentheses", "\\(");
static Terminal CLOSE_PARENTHESES("Close Parentheses", "\\)");
static Terminal ASTERISK("Asterisk", "\\*");
static Terminal PLUS("Plus", "\\+");
static Terminal LAST("Last", "last_token");

static std::vector<Terminal> terminals = { TOKENS, SKIP, QUOTE_EXPRESSION, LAST, ASTERISK, PLUS, ID, OPEN_BRACKET,
                                    CLOSE_BRACKET, OPEN_PARENTHESES, CLOSE_PARENTHESES, COMMA, COLON, SEMICOLON,
                                    RIGHT_ARROW, LEFT_CURLY_BRACKET, RIGHT_CURLY_BRACKET,};

Recognizer prediCtiveRecognizer(terminals);