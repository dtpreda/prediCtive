//
// Created by dtpreda on 28/08/22.
//

#include "SemanticCheckVisitor.h"
#include "parser/utils.h"

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

static bool visitToken(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto semanticChecker = dynamic_cast<SemanticCheckVisitor*>(context);
    if (!semanticChecker) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be SemanticCheckVisitor.";
        throw std::runtime_error(what.str());
    }

    semanticChecker->addTerminal(node->getAnnotation(SYMBOL_NAME));

    return true;
}

static bool extractRuleNames(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto semanticChecker = dynamic_cast<SemanticCheckVisitor*>(context);
    if (!semanticChecker) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be SemanticCheckVisitor.";
        throw std::runtime_error(what.str());
    }

    semanticChecker->addNonTerminal(node->getAnnotation(SYMBOL_NAME));

    return true;
}

static bool verifyRuleExpansion(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto semanticChecker = dynamic_cast<SemanticCheckVisitor*>(context);
    if (!semanticChecker) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be SemanticCheckVisitor.";
        throw std::runtime_error(what.str());
    }

    for (const auto& child: node->getChildren()) {
        if (node->getName() == "Terminal") {
            if (!semanticChecker->verifyTerminalExistence(node->getAnnotation(SYMBOL_NAME))) {
                return false;
            }
        } else if (node->getName() == "NonTerminal") {
            if (!semanticChecker->verifyNonTerminalExistence(node->getAnnotation(SYMBOL_NAME))) {
                return false;
            }
        }
    }

    return true;
}

static bool visitRules(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto semanticChecker = dynamic_cast<SemanticCheckVisitor*>(context);
    if (!semanticChecker) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be SemanticCheckVisitor.";
        throw std::runtime_error(what.str());
    }

    semanticChecker->setVisit("Rule", extractRuleNames);

    for (const auto& child: node->getChildren()) {
        semanticChecker->visit(child);
    }

    semanticChecker->setVisit("Rule", verifyRuleExpansion);

    for (const auto& child: node->getChildren()) {
        if (!semanticChecker->visit(child)) {
            std::stringstream what;
            what << "At least one rule of " << child->getAnnotation(SYMBOL_NAME) << " contains one or more undeclared symbols." << std::endl;
            throw std::runtime_error(what.str());
        }
    }

    if (!semanticChecker->verifyNonTerminalExistence("Start")) {
        std::stringstream what;
        what << "A Non-Terminal named Start should be defined to indicate the grammar's starting point." << std::endl;
        throw std::runtime_error(what.str());
    }

    return true;
}

SemanticCheckVisitor::SemanticCheckVisitor() {
    this->terminals.insert(END_OF_INPUT->getName());

    this->setVisit("SStart", descend);
    this->setVisit("Start", descend);
    this->setVisit("Tokens", descend);
    this->setVisit("Token", visitToken);
    this->setVisit("Rules", visitRules);

    this->setDefaultVisit(idle);
}

void SemanticCheckVisitor::addTerminal(const std::string &terminal) {
    auto result = this->terminals.insert(terminal);

    if (!result.second) {
        if (terminal == "EOF") {
            throw std::runtime_error("EOF is a prediCtive reserved keyword; it should not be used as a terminal name.");
        }
        std::stringstream what;
        what << "There already exists a terminal with the name " << terminal << std::endl;
        throw std::runtime_error(what.str());
    }
}

void SemanticCheckVisitor::addNonTerminal(const std::string &nonTerminal) {
    if (nonTerminal == "EOF") {
        throw std::runtime_error("EOF is a prediCtive reserved keyword; it should not be used as a non-terminal name.");
    }
    this->nonTerminals.insert(nonTerminal);
}

bool SemanticCheckVisitor::verifyTerminalExistence(const std::string &terminal) {
    return this->terminals.find(terminal) != this->terminals.end();
}

bool SemanticCheckVisitor::verifyNonTerminalExistence(const std::string &nonTerminal) {
    return this->nonTerminals.find(nonTerminal) != this->nonTerminals.end();
}