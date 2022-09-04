//
// Created by dtpreda on 23/07/22.
//

#include <sstream>
#include <utility>

#include "NonTerminal.h"

NonTerminal::NonTerminal(std::string name) : Symbol(std::move(name)) {}

void NonTerminal::addToRule(const Terminal& first, const std::shared_ptr<Terminal>& expansion) {
    const auto& expansionPtr(expansion);
    if (this->rules.find(first) == this->rules.end()) {
        this->rules.insert({first, std::vector<std::shared_ptr<Symbol>>({ expansionPtr })});
    } else {
        this->rules.find(first)->second.push_back(expansionPtr);
    }
}

void NonTerminal::addToRule(const Terminal& first, const std::shared_ptr<NonTerminal>& expansion) {
    const auto& expansionPtr(expansion);
    if (this->rules.find(first) == this->rules.end()) {
        this->rules.insert({first, std::vector<std::shared_ptr<Symbol>>({ expansionPtr })});
    } else {
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
    auto correspondingRule = this->rules.find(first);
    if (correspondingRule != this->rules.end()) {
        std::stringstream what;
        what << "Non Terminal " << this->getName() << " already contains a rule for Terminal " << first.getName() << std::endl;
        throw std::runtime_error(what.str());
    }
    if (expansion.empty()) {
        this->rules.insert({ first, std::vector<std::shared_ptr<Symbol>>() });
        this->annotations.insert({first, std::vector<std::map<std::string, std::string>>( {})});
        this->nullable = true;
        return;
    }

    this->annotations.insert({ first, std::vector<std::map<std::string, std::string>>({})});
    auto annotationVec = this->annotations.find(first);
    for(const auto & i : expansion) {
        this->addToRule(first, i);
        annotationVec->second.push_back({});
    }
}

void NonTerminal::addToRule(const Terminal &first, const std::vector<std::shared_ptr<Symbol>>& expansion,
                            const std::vector<std::map<std::string, std::string>>& annotations) {
    if (expansion.size() != annotations.size()) {
        throw std::runtime_error("Expansion size and annotations size should be the same.");
    }

    auto correspondingRule = this->rules.find(first);
    if (correspondingRule != this->rules.end()) {
        std::stringstream what;
        what << "Non Terminal " << this->getName() << " already contains a rule for Terminal " << first.getName() << std::endl;
        throw std::runtime_error(what.str());
    }
    if (expansion.empty()) {
        this->rules.insert({ first, std::vector<std::shared_ptr<Symbol>>() });
        this->annotations.insert({ first, std::vector<std::map<std::string, std::string>>({})});
        this->nullable = true;
        return;
    }

    this->annotations.insert({ first, std::vector<std::map<std::string, std::string>>({})});
    auto annotationVec = this->annotations.find(first);
    for(int i = 0; i < expansion.size(); i++) {
        this->addToRule(first, expansion[i]);
        annotationVec->second.push_back(annotations[i]);
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
    return this->nullable;
}

void NonTerminal::setNullable(bool nullable) {
    this->nullable = nullable;
}

std::vector<std::map<std::string, std::string>> NonTerminal::getAnnotation(const Terminal &first) const {
    auto correspondingRule = this->annotations.find(first);
    if (correspondingRule == this->annotations.end()) {
        std::stringstream what;
        what << "Non-terminal symbol " << this->getName() << " has no rules for terminal symbol " << first.getName()
             << "." << std::endl;
        throw std::runtime_error(what.str());
    }

    return this->annotations.find(first)->second;
}

NonTerminal::~NonTerminal() = default;
