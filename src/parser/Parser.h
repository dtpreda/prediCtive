//
// Created by dtpreda on 28/07/22.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include <vector>

#include "Recognizer.h"
#include "NonTerminal.h"

class Parser {
public:
    Parser(Recognizer recognizer, const NonTerminal& startSymbol);

    bool parse(std::string toParse) const;
private:
    Recognizer recognizer;
    NonTerminal startSymbol;
};


#endif //PARSER_PARSER_H
