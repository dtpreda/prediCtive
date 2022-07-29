//
// Created by dtpreda on 23/07/22.
//

#ifndef PARSER_NONTERMINAL_H
#define PARSER_NONTERMINAL_H

#include <map>
#include <vector>

#include "Symbol.h"
#include "Terminal.h"

class NonTerminal : public Symbol {
public:
    explicit NonTerminal(std::string name);
    NonTerminal(const NonTerminal& other);
    ~NonTerminal();
    Symbol* clone() const;


    void addToRule(const Terminal& first, const Terminal& expansion);
    void addToRule(const Terminal& first, const NonTerminal& expansion);
    std::vector<Symbol*> getRule(const Terminal& first) const;
private:
    std::map<Terminal, std::vector<Symbol*>> rules;
};


#endif //PARSER_NONTERMINAL_H
