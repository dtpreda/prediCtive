//
// Created by dtpreda on 15/08/22.
//

#include "SkipExtractorVisitor.h"

#include <sstream>
#include <stdexcept>

bool visitSkip(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto skipExtractor = dynamic_cast<SkipExtractorVisitor*>(context);
    if (!skipExtractor) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be SkipExtractorVisitor.";
        throw std::runtime_error(what.str());
    }

    std::shared_ptr<Node> regex = node->getChild(2);
    std::shared_ptr<Node> skipExpression = node->getChild(3);

    std::string regexExpr = regex->getAnnotation("consumed_token");
    regexExpr = regexExpr.substr(1, regexExpr.length() - 2);

    skipExtractor->addRegexExpression(regexExpr);

    skipExtractor->visit(skipExpression);

    node->clearChildren();
    for (auto& regexExpression: skipExtractor->getRegexExpressions()) {
        std::shared_ptr<Node> child = std::make_shared<Node>("SkipExpression");
        child->addAnnotation("regex", regexExpression);

        node->addChild(child);
    }

    return true;
}

bool visitSkipExpression(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto skipExtractor = dynamic_cast<SkipExtractorVisitor*>(context);
    if (!skipExtractor) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be SkipExtractorVisitor.";
        throw std::runtime_error(what.str());
    }

    if (node->getChildren().empty()) {
        return false;
    }

    std::shared_ptr<Node> regex = node->getChild(1);
    std::shared_ptr<Node> skipExpression = node->getChild(2);

    std::string regexExpr = regex->getAnnotation("consumed_token");
    regexExpr = regexExpr.substr(1, regexExpr.length() - 2);

    skipExtractor->addRegexExpression(regexExpr);

    skipExtractor->visit(skipExpression);

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

SkipExtractorVisitor::SkipExtractorVisitor() {
    this->setVisit("SStart", descend);
    this->setVisit("Start", descend);
    this->setVisit("Skip", visitSkip);
    this->setVisit("SkipExpression", visitSkipExpression);

    this->setDefaultVisit(idle);
}

void SkipExtractorVisitor::addRegexExpression(const std::string& regexExpression) {
    this->regexExpressions.push_back(regexExpression);
}

std::vector<std::string> SkipExtractorVisitor::getRegexExpressions() const {
    return this->regexExpressions;
}
