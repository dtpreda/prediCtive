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
    ~Terminal() override;

    std::string getRegexExpression() const;
    Terminal& operator=(const Terminal& other);

    bool isNullable() const override;
private:
    std::string regex;
};


#endif //PARSER_TERMINAL_H
