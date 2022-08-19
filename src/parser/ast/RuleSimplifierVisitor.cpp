//
// Created by dtpreda on 18/08/22.
//

#include "RuleSimplifierVisitor.h"

#include <sstream>
#include <stdexcept>

static std::string idle(Visitor<std::string>* context, const std::shared_ptr<Node>& root) {
    return "";
}

static std::string descend(Visitor<std::string>* context, const std::shared_ptr<Node>& node) {
    for (auto& child : node->getChildren()) {
        context->visit(child);
    }

    return "";
}

static std::string visitRule(Visitor<std::string>* context, const std::shared_ptr<Node>& node) {
    auto ruleSimplifier = dynamic_cast<RuleSimplifierVisitor*>(context);
    if (!ruleSimplifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be RuleSimplifierExtractor.";
        throw std::runtime_error(what.str());
    }

    std::shared_ptr<Node> expansion = node->getChild(0);

    ruleSimplifier->visit(expansion);

    node->clearChildren();
    for (auto& child : expansion->getChildren()) {
        child->setParent(node);
        node->addChild(child);
    }

    return "";
}

static std::string visitExpansion(Visitor<std::string>* context, const std::shared_ptr<Node>& node) {
    auto ruleSimplifier = dynamic_cast<RuleSimplifierVisitor*>(context);
    if (!ruleSimplifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be RuleSimplifierExtractor.";
        throw std::runtime_error(what.str());
    }

    std::shared_ptr<Node> block = node->getChild(0);
    std::shared_ptr<Node> annotation = node->getChild(1);
    std::shared_ptr<Node> nextBlock = node->getChild(2);

    ruleSimplifier->visit(block);

    if (!annotation->getChildren().empty()) {
        std::string key = ruleSimplifier->visit(annotation->getChild(1));
        std::string value = ruleSimplifier->visit(annotation->getChild(3));

        block->addAnnotation(key, value);
    }

    ruleSimplifier->addExpansionBlock(block);

    ruleSimplifier->visit(nextBlock);

    node->clearChildren();
    for (auto& child : ruleSimplifier->getExpansionBlocks()) {
        child->setParent(node);
        node->addChild(child);
    }

    ruleSimplifier->clearExpansionBlocks();

    return "";
}

static std::string visitExpansionBlock(Visitor<std::string>* context, const std::shared_ptr<Node>& node) {
    auto ruleSimplifier = dynamic_cast<RuleSimplifierVisitor*>(context);
    if (!ruleSimplifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be RuleSimplifierExtractor.";
        throw std::runtime_error(what.str());
    }

    size_t nChildren = node->getChildren().size();
    if (nChildren == 1) {
        *node = *(node->getChild(0));
        node->setName("NonTerminal");
        node->changeAnnotationKey("consumed_token", "name");
    } else if (nChildren == 3) {
        *node = *(node->getChild(1));
        node->setName("Terminal");
        node->changeAnnotationKey("consumed_token", "name");
    } else {
        std::shared_ptr<Node> expansion = node->getChild(1);
        RuleSimplifierVisitor rsv;
        rsv.visit(expansion);

        std::string closure = node->getChild(3)->getChild(0)->getAnnotation("consumed_token");

        *node = *(expansion);
        node->addAnnotation("closure", closure);
    }

    return "";
}

static std::string visitNextExpansionBlock(Visitor<std::string>* context, const std::shared_ptr<Node>& node) {
    auto ruleSimplifier = dynamic_cast<RuleSimplifierVisitor*>(context);
    if (!ruleSimplifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be RuleSimplifierExtractor.";
        throw std::runtime_error(what.str());
    }

    if (node->getChildren().empty()) {
        return "";
    }

    std::shared_ptr<Node> block = node->getChild(0);
    std::shared_ptr<Node> annotation = node->getChild(1);
    std::shared_ptr<Node> nextBlock = node->getChild(2);

    ruleSimplifier->visit(block);

    if (!annotation->getChildren().empty()) {
        std::string key = ruleSimplifier->visit(annotation->getChild(1));
        std::string value = ruleSimplifier->visit(annotation->getChild(3));

        block->addAnnotation(key, value);
    }

    ruleSimplifier->addExpansionBlock(block);

    ruleSimplifier->visit(nextBlock);

    return "";
}

static std::string visitQuoteExpression(Visitor<std::string>* context, const std::shared_ptr<Node>& node) {
    std::string annotation = node->getAnnotation("consumed_token");
    return annotation.substr(1, annotation.size() - 2);
}

static std::string visitLastToken(Visitor<std::string>* context, const std::shared_ptr<Node>& node) {
    return node->getAnnotation("consumed_token");
}

static std::string visitAnnotation(Visitor<std::string>* context, const std::shared_ptr<Node>& node) {
    return context->visit(node->getChild(0));
}

RuleSimplifierVisitor::RuleSimplifierVisitor() {
    this->setVisit("SStart", descend);
    this->setVisit("Start", descend);
    this->setVisit("Rules", descend);
    this->setVisit("Rule", visitRule);
    this->setVisit("Expansion", visitExpansion);
    this->setVisit("ExpansionBlock", visitExpansionBlock);
    this->setVisit("NextExpansionBlock", visitNextExpansionBlock);
    this->setVisit("Quote Expression", visitQuoteExpression);
    this->setVisit("AnnotationOption", visitAnnotation);
    this->setVisit("Last Token", visitLastToken);

    this->setDefaultVisit(idle);
}

std::vector<std::shared_ptr<Node>> RuleSimplifierVisitor::getExpansionBlocks() {
    return this->expansionBlocks;
}

void RuleSimplifierVisitor::addExpansionBlock(const std::shared_ptr<Node> &node) {
    this->expansionBlocks.push_back(node);
}

void RuleSimplifierVisitor::clearExpansionBlocks() {
    this->expansionBlocks.clear();
}
