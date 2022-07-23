//
// Created by dtpreda on 23/07/22.
//

#include "Symbol.h"

Symbol::Symbol(std::string name) : name(name) {}

std::string Symbol::getName() const {
    return this->name;
}
