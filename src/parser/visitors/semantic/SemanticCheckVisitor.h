//
// Created by dtpreda on 28/08/22.
//

#ifndef PARSER_SEMANTICCHECKVISITOR_H
#define PARSER_SEMANTICCHECKVISITOR_H

#include <unordered_set>

#include "parser/visitors/Visitor.h"
#include "parser/grammar/Terminal.h"
#include "parser/grammar/NonTerminal.h"

class SemanticCheckVisitor : public Visitor<bool> {
public:
    SemanticCheckVisitor();
    ~SemanticCheckVisitor() override = default;

    void addTerminal(const std::string& terminal);
    void addNonTerminal(const std::string& nonTerminal);

    bool verifyTerminalExistence(const std::string& terminal);
    bool verifyNonTerminalExistence(const std::string& nonTerminal);
private:
    std::unordered_set<std::string> terminals{};
    std::unordered_set<std::string> nonTerminals{};
};


#endif //PARSER_SEMANTICCHECKVISITOR_H
