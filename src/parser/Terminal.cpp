//
// Created by dtpreda on 23/07/22.
//

#include "Terminal.h"

#include <utility>

Terminal::Terminal(std::string name, std::string regexExpression) : Symbol(std::move(name)), regex(std::move(regexExpression)) {}

std::string Terminal::getRegexExpression() const {
    return this->regex;
}

Symbol* Terminal::clone() const {
    return new Terminal(static_cast<const Terminal&>(*this));
}

Terminal& Terminal::operator=(const Terminal& other) = default;
