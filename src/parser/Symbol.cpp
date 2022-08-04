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

Symbol& Symbol::operator=(const Symbol &other) = default;
