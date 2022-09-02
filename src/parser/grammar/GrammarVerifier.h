//
// Created by dtpreda on 02/09/22.
//

#ifndef PARSER_GRAMMARVERIFIER_H
#define PARSER_GRAMMARVERIFIER_H

#include <unordered_set>
#include <unordered_map>

#include "Terminal.h"
#include "NonTerminal.h"
#include "Symbol.h"

class GrammarVerifier {
public:
    GrammarVerifier() = default;

    bool verifyTerminalExistence(const std::string& name);
    bool verifyNonTerminalExistence(const std::string& name);

    std::shared_ptr<Terminal> getTerminal(const std::string& name);
    std::shared_ptr<NonTerminal> getNonTerminal(const std::string& name);

    void addTerminal(const std::shared_ptr<Terminal>& terminal);
    void addNonTerminal(const std::shared_ptr<NonTerminal>& nonTerminal);
    void addRule(const std::string& nonTerminalName, const std::vector<std::shared_ptr<Symbol>>& rule,
                 const std::vector<std::map<std::string, std::string>>& annotations);
private:
    std::unordered_map<std::string, std::shared_ptr<Terminal>> terminals;
    std::unordered_map<std::string, std::shared_ptr<NonTerminal>> nonTerminals;
    std::unordered_map<std::string, std::vector<std::vector<std::shared_ptr<Symbol>>>> rules;
    std::unordered_map<std::string, std::vector<std::vector<std::map<std::string, std::string>>>> annotations;
};


#endif //PARSER_GRAMMARVERIFIER_H
