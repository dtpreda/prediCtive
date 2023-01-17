//
// Created by dtpreda on 31/07/22.
//

#ifndef PARSER_UTILS_H
#define PARSER_UTILS_H

#include <memory>
#include <unordered_set>

#include "parser/grammar/Terminal.h"

extern std::shared_ptr<Terminal> END_OF_INPUT;
extern std::string CONSUMED_TOKEN;
extern std::string SYMBOL_NAME;
extern std::string CLOSURE;
extern std::string INTERMEDIATE_NON_TERMINAL_PATTERN;
extern std::string REGEX_LITERAL;
extern std::string BEFORE_START_NAME;
extern std::string LAST_TOKEN;
extern std::unordered_set<std::string> KEYWORDS;

#endif //PARSER_UTILS_H
