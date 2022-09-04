//
// Created by dtpreda on 15/08/22.
//

#ifndef PARSER_RULEEXTRACTORVISITOR_H
#define PARSER_RULEEXTRACTORVISITOR_H


#include "parser/visitors/Visitor.h"

class RuleExtractorVisitor : public Visitor<bool> {
public:
    RuleExtractorVisitor();
    ~RuleExtractorVisitor() override = default;

    std::vector<std::shared_ptr<Node>> getRuleCollector();
    void addRule(const std::shared_ptr<Node>& rule);
private:
    std::vector<std::shared_ptr<Node>> ruleCollector;
};


#endif //PARSER_RULEEXTRACTORVISITOR_H
