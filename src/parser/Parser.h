//
// Created by dtpreda on 28/07/22.
//

#ifndef PARSER_PARSER_H
#define PARSER_PARSER_H

#include <vector>
#include <memory>

#include "parser/grammar/Recognizer.h"
#include "parser/grammar/NonTerminal.h"
#include "parser/ast/Node.h"

class Parser {
public:
    Parser(Recognizer recognizer, const NonTerminal&  startSymbol);

    std::shared_ptr<Node> parse(std::string toParse);
private:
    Recognizer recognizer;
    NonTerminal startSymbol;

    std::shared_ptr<Node> parse(std::string& toParse, Terminal& currentTerminal, const std::shared_ptr<Node>& rootNode, const std::shared_ptr<Symbol>& currentSymbol);
};


#endif //PARSER_PARSER_H
