//
// Created by dtpreda on 23/07/22.
//

#include "Symbol.h"

#include <utility>

Symbol::Symbol(std::string name) : name(std::move(name)) {}

std::string Symbol::getName() const {
    return this->name;
}

bool Symbol::operator<(const Symbol& other) const {
    return this->name < other.name;
}

bool Symbol::operator==(const Symbol& other) const {
    return this->name == other.name;
}

std::unordered_set<std::shared_ptr<Symbol>> Symbol::getFirst() {
    return this->first;
}

std::unordered_set<std::shared_ptr<Symbol>> Symbol::getFollow() {
    return this->follow;
}

void Symbol::addToFirst(const std::shared_ptr<Symbol> &symbol) {
    this->first.insert(symbol);
}

void Symbol::addToFollow(const std::shared_ptr<Symbol> &symbol) {
    this->follow.insert(symbol);
}

Symbol& Symbol::operator=(const Symbol &other) = default;
