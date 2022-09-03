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

bool GrammarVerifier::isNullable(const std::vector<std::shared_ptr<Symbol>> &expansion) {
    if (!expansion.empty()) {
        for (const auto& symbol: expansion) {
            if (!symbol->isNullable()) {
                return false;
            }
        }
    }

    return true;
}

bool GrammarVerifier::updateFollow(const std::shared_ptr<NonTerminal> &nonTerminal) {
    bool hasChanges = false;

    for (const auto& setOfRules: this->rules) {
        for (const auto& rule: setOfRules.second) {
            for (int index = 0; index < rule.size(); index++) {
                if (this->nonTerminals.find(rule[index]->getName()) != this->nonTerminals.end() && rule[index]->getName() == nonTerminal->getName()) {
                    std::vector<std::shared_ptr<Symbol>> ruleRemainder = std::vector<std::shared_ptr<Symbol>>(rule.begin() + index + 1, rule.end());
                    std::unordered_set<std::shared_ptr<Symbol>> ruleRemainderFirst = computeFirst(ruleRemainder);

                    for (const auto& terminal: ruleRemainderFirst) {
                        if (nonTerminal->addToFollow(terminal)) {
                            hasChanges = true;
                        }
                    }

                    if (GrammarVerifier::isNullable(ruleRemainder)) {
                        for (const auto& terminal: this->nonTerminals.find(setOfRules.first)->second->getFollow()) {
                            if (nonTerminal->addToFollow(terminal)) {
                                hasChanges = true;
                            }
                        }
                    }
                }
            }
        }
    }

    return hasChanges;
}

std::unordered_set<std::shared_ptr<Symbol>> GrammarVerifier::computeFirst(const std::vector<std::shared_ptr<Symbol>>& expansion) {
    std::unordered_set<std::shared_ptr<Symbol>> first;

    for (const auto& symbol: expansion) {
        for (const auto& terminal: symbol->getFirst()) {
            first.insert(terminal);
        }

        if (!symbol->isNullable()) {
            break;
        }
    }

    return first;
}

void GrammarVerifier::computeSets() {
    bool hasChanges = true;

    for (const auto& terminal: this->terminals) {
        terminal.second->addToFirst(terminal.second);
    }

    while (hasChanges) {
        hasChanges = false;

        for (const auto& setOfRules: this->rules) {
            if (!nonTerminals.find(setOfRules.first)->second->isNullable()) {
                for (const auto& rule: setOfRules.second) {
                    if (this->isNullable(rule)) {
                        hasChanges = true;
                        nonTerminals.find(setOfRules.first)->second->setNullable(true);
                        break;
                    }
                }
            }
        }

        for (const auto& setOfRules: this->rules) {
            std::shared_ptr<NonTerminal> nonTerminal = nonTerminals.find(setOfRules.first)->second;
            for (const auto& rule: setOfRules.second) {
                for (const auto& symbol: rule) {
                    for (const auto& terminal : symbol->getFirst()) {
                        if (nonTerminal->addToFirst(terminal)) {
                            hasChanges = true;
                        }
                    }
                    if (!symbol->isNullable()) {
                        break;
                    }
                }
            }
        }
    }

    hasChanges = true;

    while (hasChanges) {
        hasChanges = false;

        for (const auto& nonTerminalPair: this->nonTerminals) {
            std::shared_ptr<NonTerminal> nonTerminal = nonTerminalPair.second;

            if (this->updateFollow(nonTerminal)) {
                hasChanges = true;
            }
        }
    }
}
