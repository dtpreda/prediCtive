//
// Created by dtpreda on 02/09/22.
//

#include "GrammarVerifier.h"

bool GrammarVerifier::verifyTerminalExistence(const std::string &name) {
    return this->terminals.find(name) != this->terminals.end();
}

bool GrammarVerifier::verifyNonTerminalExistence(const std::string &name) {
    return this->nonTerminals.find(name) != this->nonTerminals.end();
}

std::shared_ptr<Terminal> GrammarVerifier::getTerminal(const std::string &name) {
    return this->terminals.find(name)->second;
}

std::shared_ptr<NonTerminal> GrammarVerifier::getNonTerminal(const std::string &name) {
    return this->nonTerminals.find(name)->second;
}

void GrammarVerifier::addTerminal(const std::shared_ptr<Terminal> &terminal) {
    this->terminals.insert({terminal->getName(), terminal});
}

void GrammarVerifier::addNonTerminal(const std::shared_ptr<NonTerminal> &nonTerminal) {
    this->nonTerminals.insert({nonTerminal->getName(), nonTerminal});
}

void GrammarVerifier::addRule(const std::string& nonTerminalName, const std::vector<std::shared_ptr<Symbol>>& rule,
                              const std::vector<std::map<std::string, std::string>>& annotations) {
    if (this->rules.find(nonTerminalName) != this->rules.end()) {
        this->rules.find(nonTerminalName)->second.push_back(rule);
        this->annotations.find(nonTerminalName)->second.push_back(annotations);
    } else {
        this->rules.insert({nonTerminalName, std::vector<std::vector<std::shared_ptr<Symbol>>>({rule})});
        this->annotations.insert({nonTerminalName, std::vector<std::vector<std::map<std::string, std::string>>>({annotations})});
    }
}
