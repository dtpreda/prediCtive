//
// Created by dtpreda on 05/08/22.
//

#include "TokenExtractorVisitor.h"

#include <sstream>
#include <stdexcept>

static bool visitTokens(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto parseTreeFlatenner = dynamic_cast<TokenExtractorVisitor*>(context);
    if (!parseTreeFlatenner) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be ParseTreeFlatennerVisitor.";
        throw std::runtime_error(what.str());
    }

    std::shared_ptr<Node> token = node->getChild(2);
    context->visit(token);

    std::shared_ptr<Node> nextToken = node->getChild(3);
    if (!nextToken->getChildren().empty()) {
        context->visit(nextToken);
    }

    node->clearChildren();
    for (auto& child : parseTreeFlatenner->getTokenCollector()) {
        child->clearChildren();
        node->addChild(child);
    }

    return true;
}

static bool visitToken(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto parseTreeFlatenner = dynamic_cast<TokenExtractorVisitor*>(context);
    if (!parseTreeFlatenner) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be ParseTreeFlatennerVisitor.";
        throw std::runtime_error(what.str());
    }

    std::shared_ptr<Node> identifier = node->getChild(0);
    std::shared_ptr<Node> regex = node->getChild(2);

    node->addAnnotation("name", identifier->getAnnotation("consumed_token"));

    std::string regexExpr = regex->getAnnotation("consumed_token");
    regexExpr = regexExpr.substr(1, regexExpr.length() - 2);

    node->addAnnotation("regex", regexExpr);

    parseTreeFlatenner->addToken(node);
    return true;
}

static bool visitNextToken(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto parseTreeFlatenner = dynamic_cast<TokenExtractorVisitor*>(context);
    if (!parseTreeFlatenner) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be ParseTreeFlatennerVisitor.";
        throw std::runtime_error(what.str());
    }

    std::shared_ptr<Node> token = node->getChild(1);
    context->visit(token);

    std::shared_ptr<Node> nextToken = node->getChild(2);
    if (!nextToken->getChildren().empty()) {
        context->visit(nextToken);
    }

    return true;
}

static bool descend(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    for (auto& child : node->getChildren()) {
        context->visit(child);
    }

    return true;
}

static bool idle(Visitor<bool> *context, const std::shared_ptr<Node>& node) {
    return true;
}

TokenExtractorVisitor::TokenExtractorVisitor() {
    this->tokenCollector = std::vector<std::shared_ptr<Node>>({});

    this->setVisit("Tokens", visitTokens);
    this->setVisit("Token", visitToken);
    this->setVisit("NextToken", visitNextToken);
    this->setVisit("SStart", descend);
    this->setVisit("Start", descend);

    this->setDefaultVisit(idle);
}

std::vector<std::shared_ptr<Node>> TokenExtractorVisitor::getTokenCollector() {
    return this->tokenCollector;
}

void TokenExtractorVisitor::addToken(const std::shared_ptr<Node>& token) {
    this->tokenCollector.push_back(token);
}
