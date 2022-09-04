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

bool Symbol::addToFirst(const std::shared_ptr<Symbol> &symbol) {
    auto result = this->first.insert(symbol);
    return result.second;
}

bool Symbol::addToFollow(const std::shared_ptr<Symbol> &symbol) {
    auto result = this->follow.insert(symbol);
    return result.second;
}

Symbol& Symbol::operator=(const Symbol &other) = default;
