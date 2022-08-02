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

void NonTerminal::addToRule(const Terminal &first, const Symbol* expansion) {
    auto* terminal = dynamic_cast<const Terminal*>(expansion);

    if (terminal) {
        this->addToRule(first, *terminal);
    } else {
        auto* nonTerminal = dynamic_cast<const NonTerminal*>(expansion);

        if (nonTerminal) {
            this->addToRule(first, *nonTerminal);
        } else {
            throw std::runtime_error("Corrupted Symbol object.");
        }
    }
}

void NonTerminal::addToRule(const Terminal &first, const std::vector<Symbol*>& expansion) {
    for(auto &expansionElement : expansion) {
        this->addToRule(first, expansionElement);
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

NonTerminal::~NonTerminal() {
    for (auto & rule : this->rules) {
        while(!rule.second.empty()) {
            delete rule.second.back();
            rule.second.pop_back();
        }
    }
}

NonTerminal::NonTerminal(const NonTerminal& other) : Symbol(other.getName()) {
    for (auto& rule : other.rules) {
        std::vector<Symbol*> ruleExpansion;
        for (auto& symbol : rule.second) {
            ruleExpansion.push_back(symbol->clone());
        }
        this->rules.insert({rule.first, ruleExpansion});
    }
}

Symbol* NonTerminal::clone() const {
    return new NonTerminal(static_cast<const NonTerminal&>(*this));
}
