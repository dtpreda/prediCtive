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
private:
    friend bool visitTokens(Visitor<bool>* context, Node& node);
    friend bool visitToken(Visitor<bool>* context, Node& node);
    friend bool visitNextToken(Visitor<bool>* context, Node& node);
    friend bool descend(Visitor<bool>* context, Node& node);

    std::vector<Node> tokenCollector;
};


#endif //PARSER_TOKENEXTRACTORVISITOR_H
