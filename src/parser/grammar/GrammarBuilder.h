//
// Created by dtpreda on 02/09/22.
//

#ifndef PARSER_GRAMMARBUILDER_H
#define PARSER_GRAMMARBUILDER_H

#include <unordered_set>
#include <unordered_map>

#include "Terminal.h"
#include "NonTerminal.h"
#include "Symbol.h"

class GrammarBuilder {
public:
    GrammarBuilder() = default;

    bool verifyTerminalExistence(const std::string& name);
    bool verifyNonTerminalExistence(const std::string& name);

    std::shared_ptr<Terminal> getTerminal(const std::string& name);
    std::shared_ptr<NonTerminal> getNonTerminal(const std::string& name);

    void addTerminal(const std::shared_ptr<Terminal>& terminal);
    void addNonTerminal(const std::shared_ptr<NonTerminal>& nonTerminal);
    void addRule(const std::string& nonTerminalName, const std::vector<std::shared_ptr<Symbol>>& rule,
                 const std::vector<std::map<std::string, std::string>>& annotations);

    void computeSets();
    void buildGrammar();
private:
    std::unordered_map<std::string, std::shared_ptr<Terminal>> terminals;
    std::vector<std::shared_ptr<Terminal>> ordered_terminals;
    std::unordered_map<std::string, std::shared_ptr<NonTerminal>> nonTerminals;
    std::unordered_map<std::string, std::vector<std::vector<std::shared_ptr<Symbol>>>> rules;
    std::unordered_map<std::string, std::vector<std::vector<std::map<std::string, std::string>>>> annotations;

    static bool isNullable(const std::vector<std::shared_ptr<Symbol>>& expansion);
    bool updateFollow(const std::shared_ptr<NonTerminal>& nonTerminal);
    static std::unordered_set<std::shared_ptr<Symbol>> computeFirst(const std::vector<std::shared_ptr<Symbol>>& expansion);
};


#endif //PARSER_GRAMMARBUILDER_H
