//
// Created by dtpreda on 23/07/22.
//

#include "Terminal.h"

Terminal::Terminal(std::string name, std::string regexExpression) : name(name), regex(std::regex(regexExpression)) {}

std::string Terminal::getName() const {
    return this->name;
}

std::regex Terminal::getRegex() const {
    return this->regex;
}
