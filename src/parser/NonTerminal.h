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
    NonTerminal(std::string name);

    void addRule(const Terminal& first, const std::vector<Symbol>& expansion);
    std::vector<Symbol> getRule(const Terminal& first) const;
private:
    std::map<Terminal, std::vector<Symbol>> rules;
};


#endif //PARSER_NONTERMINAL_H
