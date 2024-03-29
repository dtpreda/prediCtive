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
    void addToRule(const Terminal &first, const std::vector<std::shared_ptr<Symbol>>& expansion,
                   const std::vector<std::map<std::string, std::string>>& annotations);
    std::vector<std::shared_ptr<Symbol>> getRule(const Terminal& first) const;
    std::vector<std::map<std::string, std::string>> getAnnotation(const Terminal& first) const;

    bool isNullable() const override;
    void setNullable(bool nullable);
private:
    std::map<Terminal, std::vector<std::shared_ptr<Symbol>>> rules;
    std::map<Terminal, std::vector<std::map<std::string, std::string>>> annotations;
    bool nullable = false;
};

namespace std {
    template<> struct hash<NonTerminal>
    {
        std::size_t operator()(const NonTerminal& t) const noexcept
        {
            std::hash<string> hasher;
            return hasher(t.getName());
        }
    };
}


#endif //PARSER_NONTERMINAL_H
