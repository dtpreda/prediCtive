//
// Created by dtpreda on 18/08/22.
//

#ifndef PARSER_RULESIMPLIFIERVISITOR_H
#define PARSER_RULESIMPLIFIERVISITOR_H

#include "Visitor.h"

class RuleSimplifierVisitor : public Visitor<std::string> {
public:
    RuleSimplifierVisitor();
    ~RuleSimplifierVisitor() override = default;

    std::vector<std::shared_ptr<Node>> getExpansionBlocks();
    void addExpansionBlock(const std::shared_ptr<Node>& node);
    void clearExpansionBlocks();
private:
    std::vector<std::shared_ptr<Node>> expansionBlocks;
};


#endif //PARSER_RULESIMPLIFIERVISITOR_H
