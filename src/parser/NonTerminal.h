//
// Created by dtpreda on 23/07/22.
//

#ifndef PARSER_NONTERMINAL_H
#define PARSER_NONTERMINAL_H

#include <map>
#include <vector>

#include "Symbol.h"
#include "Terminal.h"
#include <memory>

class NonTerminal : public Symbol {
public:
    explicit NonTerminal(std::string name);
    ~NonTerminal() override;

    void addToRule(const Terminal &first, const std::shared_ptr<Symbol>& expansion);
    void addToRule(const Terminal& first, const std::shared_ptr<Terminal>& expansion);
    void addToRule(const Terminal& first, const std::shared_ptr<NonTerminal>& expansion);
    void addToRule(const Terminal &first, const std::vector<std::shared_ptr<Symbol>>& expansion);
    std::vector<std::shared_ptr<Symbol>> getRule(const Terminal& first) const;

    bool isNullable() const override;
private:
    std::map<Terminal, std::vector<std::shared_ptr<Symbol>>> rules;
    int nullExpansions = 0;
};


#endif //PARSER_NONTERMINAL_H
