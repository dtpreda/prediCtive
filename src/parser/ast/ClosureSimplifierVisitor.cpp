//
// Created by dtpreda on 19/08/22.
//

#include "ClosureSimplifierVisitor.h"

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
    auto closureSimplifier = dynamic_cast<ClosureSimplifierVisitor*>(context);
    if (!closureSimplifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be RuleExtractorVisitor.";
        throw std::runtime_error(what.str());
    }

    for (auto& child: node->getChildren()) {
        closureSimplifier->visit(child);
    }

    for (auto& rule: closureSimplifier->getNewRules()) {
        node->addChild(rule);
    }

    return true;
}

static bool visitRule(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto closureSimplifier = dynamic_cast<ClosureSimplifierVisitor*>(context);
    if (!closureSimplifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be RuleExtractorVisitor.";
        throw std::runtime_error(what.str());
    }

    for (auto& child: node->getChildren()) {
        if (child->getName() == "Expansion") {
            closureSimplifier->visit(child);
            node->replaceChild(child, closureSimplifier->getNewBlocks());
            closureSimplifier->clearNewBlocks();
        }
    }

    return true;
}

static bool visitExpansion(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto closureSimplifier = dynamic_cast<ClosureSimplifierVisitor*>(context);
    if (!closureSimplifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be RuleExtractorVisitor.";
        throw std::runtime_error(what.str());
    }

    std::string closure = node->getAnnotation("closure");
    node->deleteAnnotation("closure");

    std::string nonTerminalName = "Intermediate_NonTerminal_";
    nonTerminalName.append(std::to_string(closureSimplifier->getAndIntermediateCounter()));

    std::shared_ptr<Node> closureRule = std::make_shared<Node>("Rule");
    std::shared_ptr<Node> emptyRule = std::make_shared<Node>("Rule");
    closureRule->addAnnotation("name", nonTerminalName);
    emptyRule->addAnnotation("name", nonTerminalName);

    for (auto& child: node->getChildren()) {
        std::shared_ptr<Node> newChild = std::make_shared<Node>(*child);
        newChild->setParent(closureRule);
        closureRule->addChild(newChild);
    }


    if (closure == "+") {
        for (auto& child: node->getChildren()) {
            child->setParent(node->getParent());
            closureSimplifier->addNewBlock(child);
        }
    }

    node->setName("NonTerminal");
    node->addAnnotation("name", nonTerminalName);
    node->clearChildren();
    closureRule->addChild(node);

    closureSimplifier->addNewRule(closureRule);
    closureSimplifier->addNewRule(emptyRule);

    closureSimplifier->addNewBlock(node);

    return true;
}

ClosureSimplifierVisitor::ClosureSimplifierVisitor() {
    this->setVisit("SStart", descend);
    this->setVisit("Start", descend);
    this->setVisit("Rules", visitRules);
    this->setVisit("Rule", visitRule);
    this->setVisit("Expansion", visitExpansion);

    this->setDefaultVisit(idle);
}

std::vector<std::shared_ptr<Node>> ClosureSimplifierVisitor::getNewRules() {
    return this->newRules;
}

void ClosureSimplifierVisitor::addNewRule(const std::shared_ptr<Node> &node) {
    this->newRules.push_back(node);
}

std::vector<std::shared_ptr<Node>> ClosureSimplifierVisitor::getNewBlocks() {
    return this->newBlocks;
}

void ClosureSimplifierVisitor::addNewBlock(const std::shared_ptr<Node> &node) {
    this->newBlocks.push_back(node);
}

void ClosureSimplifierVisitor::clearNewBlocks() {
    this->newBlocks.clear();
}

int ClosureSimplifierVisitor::getAndIntermediateCounter() {
    return intermediateCounter++;
}
