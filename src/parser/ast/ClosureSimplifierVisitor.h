//
// Created by dtpreda on 19/08/22.
//

#ifndef PARSER_CLOSURESIMPLIFIERVISITOR_H
#define PARSER_CLOSURESIMPLIFIERVISITOR_H

#include "Visitor.h"

#include <vector>
#include <memory>

class ClosureSimplifierVisitor :  public Visitor<bool> {
public:
    ClosureSimplifierVisitor();
    ~ClosureSimplifierVisitor() override = default;

    std::vector<std::shared_ptr<Node>> getNewRules();
    void addNewRule(const std::shared_ptr<Node>& node);

    std::vector<std::shared_ptr<Node>> getNewBlocks();
    void addNewBlock(const std::shared_ptr<Node>& node);
    void clearNewBlocks();

    int getAndIntermediateCounter();
private:
    std::vector<std::shared_ptr<Node>> newRules;
    std::vector<std::shared_ptr<Node>> newBlocks;
    int intermediateCounter = 0;
};


#endif //PARSER_CLOSURESIMPLIFIERVISITOR_H
