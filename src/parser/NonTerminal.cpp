//
// Created by dtpreda on 23/07/22.
//

#include <sstream>
#include <utility>

#include "NonTerminal.h"

NonTerminal::NonTerminal(std::string name) : Symbol(std::move(name)) {}

void NonTerminal::addToRule(const Terminal& first, const Terminal& expansion) {
    Symbol *expansionPtr = new Terminal(expansion);
    if (this->rules.find(first) == this->rules.end()) {
        this->rules.insert({first, std::vector<Symbol*>({ expansionPtr })});
    } else {
        this->rules.find(first)->second.push_back(expansionPtr);
    }
}

void NonTerminal::addToRule(const Terminal& first, const NonTerminal& expansion) {
    Symbol *expansionPtr = new NonTerminal(expansion);
    if (this->rules.find(first) == this->rules.end()) {
        this->rules.insert({first, std::vector<Symbol*>({ expansionPtr })});
    } else {
        this->rules.find(first)->second.push_back(expansionPtr);
    }
}

std::vector<Symbol*> NonTerminal::getRule(const Terminal& first) const {
    auto correspondingRule = this->rules.find(first);
    if (correspondingRule == this->rules.end()) {
        std::stringstream what;
        what << "Non-terminal symbol " << this->getName() << " has no rules for terminal symbol " << first.getName()
             << "." << std::endl;
        throw std::runtime_error(what.str());
    }

    return this->rules.find(first)->second;
}
