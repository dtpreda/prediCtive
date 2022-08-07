//
// Created by dtpreda on 05/08/22.
//

#include "TokenExtractorVisitor.h"

#include <sstream>
#include <stdexcept>

bool visitTokens(Visitor<bool>* context, Node &node) {
    auto parseTreeFlatenner = dynamic_cast<TokenExtractorVisitor*>(context);
    if (!parseTreeFlatenner) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be ParseTreeFlatennerVisitor.";
        throw std::runtime_error(what.str());
    }

    Node token = node.getChild(2);
    context->visit(token);

    Node nextToken = node.getChild(3);
    if (!nextToken.getChildren().empty()) {
        context->visit(nextToken);
    }

    node.clearChildren();
    for (auto& child : parseTreeFlatenner->tokenCollector) {
        child.clearChildren();
        node.addChild(child);
    }

    return true;
}

bool visitToken(Visitor<bool>* context, Node &node) {
    auto parseTreeFlatenner = dynamic_cast<TokenExtractorVisitor*>(context);
    if (!parseTreeFlatenner) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be ParseTreeFlatennerVisitor.";
        throw std::runtime_error(what.str());
    }

    Node identifier = node.getChild(0);
    Node regex = node.getChild(2);

    node.addAnnotation("name", identifier.getAnnotation("consumed_token"));

    std::string regexExpr = regex.getAnnotation("consumed_token");
    regexExpr = regexExpr.substr(1, regexExpr.length() - 2);

    node.addAnnotation("regex", regexExpr);

    parseTreeFlatenner->tokenCollector.push_back(node);
    return true;
}

bool visitNextToken(Visitor<bool>* context, Node &node) {
    auto parseTreeFlatenner = dynamic_cast<TokenExtractorVisitor*>(context);
    if (!parseTreeFlatenner) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be ParseTreeFlatennerVisitor.";
        throw std::runtime_error(what.str());
    }

    Node token = node.getChild(1);
    context->visit(token);

    Node nextToken = node.getChild(2);
    if (!nextToken.getChildren().empty()) {
        context->visit(nextToken);
    }

    return true;
}

bool descend(Visitor<bool>* context, Node &node) {
    for (auto& child : node.getChildren()) {
        context->visit(*child);
    }

    return true;
}

TokenExtractorVisitor::TokenExtractorVisitor() {
    this->tokenCollector = std::vector<Node>({});

    this->setVisit("Tokens", visitTokens);
    this->setVisit("Token", visitToken);
    this->setVisit("NextToken", visitNextToken);

    this->setDefaultVisit(descend);
}
