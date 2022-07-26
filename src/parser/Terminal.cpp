//
// Created by dtpreda on 23/07/22.
//

#include "Terminal.h"

std::string Terminal::NULL_TERMINAL = "NULL TERMINAL";

Terminal::Terminal(std::string name, std::string regexExpression) : Symbol(name), regex(regexExpression) {}

std::string Terminal::getRegexExpression() const {
    return this->regex;
}
