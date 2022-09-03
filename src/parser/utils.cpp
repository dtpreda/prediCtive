//
// Created by dtpreda on 02/08/22.
//

#include <memory>

#include "utils.h"

std::shared_ptr<Terminal> END_OF_INPUT = std::make_shared<Terminal>("EOF", "$");
std::string CONSUMED_TOKEN = "consumed_token";
std::string SYMBOL_NAME = "symbol_name";
std::string CLOSURE = "closure";
std::string REGEX_LITERAL = "regex";
std::string INTERMEDIATE_NON_TERMINAL_PATTERN = "Intermediate_NonTerminal_";

std::unordered_set<std::string> KEYWORDS = std::unordered_set<std::string>({CONSUMED_TOKEN, SYMBOL_NAME, CLOSURE, REGEX_LITERAL});