//
// Created by dtpreda on 23/07/22.
//

#ifndef PARSER_SYMBOL_H
#define PARSER_SYMBOL_H

#include <string>

class Symbol {
public:
    Symbol(std::string name);

    std::string getName() const;
private:
    const std::string name;
};


#endif //PARSER_SYMBOL_H
