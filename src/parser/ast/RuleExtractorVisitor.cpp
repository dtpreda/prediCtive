//
//
// Created by dtpreda on 15/08/22.

#include "RuleExtractorVisitor.h"

#include <sstream>
#include <stdexcept>

static bool idle(Visitor<bool>* context, const std::shared_ptr<Node>& root) {
    return true;
}

static bool descend(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    for (auto& child : node->getChildren()) {
        context->visit(child);
    }

    return true;
}

static bool visitRules(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto ruleSimplifier = dynamic_cast<RuleExtractorVisitor*>(context);
    if (!ruleSimplifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be RuleSimplifierExtractor.";
        throw std::runtime_error(what.str());
    }

    std::shared_ptr<Node> nextRule = node->getChild(4);
    std::shared_ptr<Node> expansion = node->getChild(2);

    std::string leftSideName = node->getChild(0)->getAnnotation("consumed_token");

    std::shared_ptr<Node> rule = std::make_shared<Node>("Rule");
    rule->addAnnotation("name", leftSideName);
    rule->addChild(expansion);
    expansion->setParent(rule);
    ruleSimplifier->addRule(rule);

    ruleSimplifier->visit(nextRule);

    node->clearChildren();
    for (auto& child: ruleSimplifier->getRuleCollector()) {
        child->setParent(node);
        node->addChild(child);
    }

    return true;
}

static bool visitNextRule(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    if (node->getChildren().empty()) {
        return false;
    }

    auto ruleSimplifier = dynamic_cast<RuleExtractorVisitor*>(context);
    if (!ruleSimplifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be RuleSimplifierExtractor.";
        throw std::runtime_error(what.str());
    }

    std::shared_ptr<Node> nextRule = node->getChild(4);

    std::string leftSideName = node->getChild(0)->getAnnotation("consumed_token");

    std::shared_ptr<Node> rule = std::make_shared<Node>("Rule");
    rule->addAnnotation("name", leftSideName);
    rule->addChild(node->getChild(2));
    ruleSimplifier->addRule(rule);

    ruleSimplifier->visit(nextRule);

    return true;
}



RuleExtractorVisitor::RuleExtractorVisitor() {
    this->setVisit("SStart", descend);
    this->setVisit("Start", descend);
    this->setVisit("Rules", visitRules);
    this->setVisit("NextRule", visitNextRule);

    this->setDefaultVisit(idle);
}

std::vector<std::shared_ptr<Node>> RuleExtractorVisitor::getRuleCollector() {
    return this->ruleCollector;
}

void RuleExtractorVisitor::addRule(const std::shared_ptr<Node> &rule) {
    this->ruleCollector.push_back(rule);
}
