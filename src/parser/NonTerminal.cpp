//
// Created by dtpreda on 23/07/22.
//

#include <sstream>
#include <utility>

#include "NonTerminal.h"

NonTerminal::NonTerminal(std::string name) : Symbol(std::move(name)) {}

void NonTerminal::addToRule(const Terminal& first, const std::shared_ptr<Terminal>& expansion) {
    auto expansionPtr(expansion);
    if (this->rules.find(first) == this->rules.end()) {
        this->rules.insert({first, std::vector<std::shared_ptr<Symbol>>({ expansionPtr })});
    } else {
        if (this->rules.find(first)->second.empty()) {
            this->nullExpansions -= 1;
        }
        this->rules.find(first)->second.push_back(expansionPtr);
    }
}

void NonTerminal::addToRule(const Terminal& first, const std::shared_ptr<NonTerminal>& expansion) {
    auto expansionPtr(expansion);
    if (this->rules.find(first) == this->rules.end()) {
        this->rules.insert({first, std::vector<std::shared_ptr<Symbol>>({ expansionPtr })});
    } else {
        if (this->rules.find(first)->second.empty()) {
            this->nullExpansions -= 1;
        }
        this->rules.find(first)->second.push_back(expansionPtr);
    }
}

void NonTerminal::addToRule(const Terminal &first, const std::shared_ptr<Symbol>& expansion) {
    auto terminal = std::dynamic_pointer_cast<Terminal>(expansion);

    if (terminal) {
        this->addToRule(first, terminal);
    } else {
        auto nonTerminal = std::dynamic_pointer_cast<NonTerminal>(expansion);

        if (nonTerminal) {
            this->addToRule(first, nonTerminal);
        } else {
            throw std::runtime_error("Corrupted Symbol object.");
        }
    }
}

void NonTerminal::addToRule(const Terminal &first, const std::vector<std::shared_ptr<Symbol>>& expansion) {
    if (expansion.empty()) {
        this->rules.insert({ first, std::vector<std::shared_ptr<Symbol>>() });
        this->nullExpansions += 1;
    }
    for(auto &expansionElement : expansion) {
        this->addToRule(first, expansionElement);
    }
}

std::vector<std::shared_ptr<Symbol>> NonTerminal::getRule(const Terminal& first) const {
    auto correspondingRule = this->rules.find(first);
    if (correspondingRule == this->rules.end()) {
        std::stringstream what;
        what << "Non-terminal symbol " << this->getName() << " has no rules for terminal symbol " << first.getName()
             << "." << std::endl;
        throw std::runtime_error(what.str());
    }

    return this->rules.find(first)->second;
}

bool NonTerminal::isNullable() const {
    return this->nullExpansions > 0;
}

NonTerminal::~NonTerminal() = default;
