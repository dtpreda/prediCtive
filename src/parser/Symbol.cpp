//
// Created by dtpreda on 23/07/22.
//

#include "Symbol.h"

Symbol::Symbol(std::string name) : name(name) {}

std::string Symbol::getName() const {
    return this->name;
}

bool Symbol::operator<(const Symbol other) const {
    return this->name < other.name;
}

bool Symbol::operator==(const Symbol other) const {
    return this->name == other.name;
}
