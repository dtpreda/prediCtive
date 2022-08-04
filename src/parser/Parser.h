//
// Created by dtpreda on 28/07/22.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include <vector>
#include <memory>

#include "Recognizer.h"
#include "NonTerminal.h"
#include "Node.h"

class Parser {
public:
    Parser(Recognizer recognizer, const NonTerminal&  startSymbol);

    Node parse(std::string toParse);
private:
    Recognizer recognizer;
    NonTerminal startSymbol;

    Node parse(std::string& toParse, Terminal& currentTerminal, Node rootNode, const std::shared_ptr<Symbol>& currentSymbol);
};


#endif //PARSER_PARSER_H
