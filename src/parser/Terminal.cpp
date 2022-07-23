//
// Created by dtpreda on 23/07/22.
//

#include "Terminal.h"

Terminal::Terminal(std::string name, std::string regexExpression) : Symbol(name), regex(std::regex(regexExpression)) {}

std::regex Terminal::getRegex() const {
    return this->regex;
}
