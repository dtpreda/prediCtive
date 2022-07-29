//
// Created by dtpreda on 23/07/22.
//

#ifndef PARSER_SYMBOL_H
#define PARSER_SYMBOL_H

#include <string>

class Symbol {
public:
    explicit Symbol(std::string name);
    virtual Symbol* clone() const = 0;

    std::string getName() const;
    bool operator<(const Symbol& other) const;
    bool operator==(const Symbol& other) const;
    Symbol& operator=(const Symbol& other);
private:
    std::string name;
};


#endif //PARSER_SYMBOL_H
