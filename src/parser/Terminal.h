//
// Created by dtpreda on 23/07/22.
//

#ifndef PARSER_TERMINAL_H
#define PARSER_TERMINAL_H

#include <string>
#include <regex>

class Terminal {
public:
    Terminal(std::string name, std::string regexExpression);

    std::string getName() const;
    std::regex getRegex() const;
private:
    const std::string name;
    const std::regex regex;
};


#endif //PARSER_TERMINAL_H
