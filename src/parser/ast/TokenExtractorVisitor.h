//
// Created by dtpreda on 05/08/22.
//

#ifndef PARSER_TOKENEXTRACTORVISITOR_H
#define PARSER_TOKENEXTRACTORVISITOR_H

#include "Visitor.h"
#include "Node.h"

class TokenExtractorVisitor : public Visitor<bool> {
public:
    TokenExtractorVisitor();
    ~TokenExtractorVisitor() override = default;

    std::vector<std::shared_ptr<Node>> getTokenCollector();
    void addToken(const std::shared_ptr<Node>& token);
private:
    std::vector<std::shared_ptr<Node>> tokenCollector;
};


#endif //PARSER_TOKENEXTRACTORVISITOR_H
