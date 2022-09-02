//
// Created by dtpreda on 23/07/22.
//

#ifndef PARSER_SYMBOL_H
#define PARSER_SYMBOL_H

#include <string>
#include <unordered_set>
#include <memory>

class Symbol {
public:
    explicit Symbol(std::string name);
    virtual ~Symbol() = default;
    std::string getName() const;

    bool operator<(const Symbol& other) const;
    bool operator==(const Symbol& other) const;
    Symbol& operator=(const Symbol& other);

    virtual bool isNullable() const = 0;

    std::unordered_set<std::shared_ptr<Symbol>> getFirst();
    std::unordered_set<std::shared_ptr<Symbol>> getFollow();

    void addToFirst(const std::shared_ptr<Symbol>& symbol);
    void addToFollow(const std::shared_ptr<Symbol>& symbol);
private:
    std::string name;
    std::unordered_set<std::shared_ptr<Symbol>> first;
    std::unordered_set<std::shared_ptr<Symbol>> follow;
};


#endif //PARSER_SYMBOL_H
