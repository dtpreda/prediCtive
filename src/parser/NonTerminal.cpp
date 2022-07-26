//
// Created by dtpreda on 23/07/22.
//

#include <sstream>

#include "NonTerminal.h"

NonTerminal::NonTerminal(std::string name) : Symbol(name) {}

void NonTerminal::addRule(const Terminal& first, const std::vector<Symbol>& expansion) {
    if (this->rules.find(first) != this->rules.end()) {
        std::stringstream what;
        what << "Non-terminal symbol " << this->getName() << " can't contain more than one rule for terminal symbol "
             << first.getName() << "." << std::endl;
        throw std::runtime_error(what.str());
    }

    this->rules.insert({first , expansion});
}

std::vector<Symbol> NonTerminal::getRule(const Terminal &first) const {
    auto correspondingRule = this->rules.find(first);
    if (correspondingRule == this->rules.end()) {
        std::stringstream what;
        what << "Non-terminal symbol " << this->getName() << " has no rules for terminal symbol " << first.getName()
             << "." << std::endl;
        throw std::runtime_error(what.str());
    }
    return this->rules.find(first)->second;
}
