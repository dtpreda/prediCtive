//
// Created by dtpreda on 23/07/22.
//

#ifndef PARSER_TERMINAL_H
#define PARSER_TERMINAL_H

#include <string>
#include <regex>

#include "Symbol.h"

class Terminal : public Symbol {
public:
    Terminal(std::string name, std::string regexExpression);

    std::regex getRegex() const;
private:
    const std::regex regex;
};


#endif //PARSER_TERMINAL_H
