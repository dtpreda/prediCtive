//
// Created by dtpreda on 02/09/22.
//

#include "GrammarBuilderVisitor.h"

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
    auto grammarBuilder = dynamic_cast<GrammarBuilderVisitor*>(context);
    if (!grammarBuilder) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be GrammarBuilderVisitor.";
        throw std::runtime_error(what.str());
    }

    std::shared_ptr<Terminal> terminal = std::make_shared<Terminal>(node->getAnnotation(SYMBOL_NAME), node->getAnnotation(REGEX_LITERAL));

    grammarBuilder->grammarBuilder.addTerminal(terminal);

    return true;
}

static void cleanAnnotations(std::map<std::string, std::string>& annotations) {
    for (const auto& keyword: KEYWORDS) {
        if (annotations.find(keyword) != annotations.end()) {
            annotations.erase(keyword);
        }
    }
}

static bool visitRule(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto grammarBuilder = dynamic_cast<GrammarBuilderVisitor*>(context);
    if (!grammarBuilder) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be GrammarBuilderVisitor.";
        throw std::runtime_error(what.str());
    }

    std::string nonTerminalName = node->getAnnotation(SYMBOL_NAME);

    if (!grammarBuilder->grammarBuilder.verifyNonTerminalExistence(nonTerminalName)) {
        std::shared_ptr<NonTerminal> nonTerminal = std::make_shared<NonTerminal>(nonTerminalName);

        grammarBuilder->grammarBuilder.addNonTerminal(nonTerminal);
    }

    std::vector<std::shared_ptr<Symbol>> rule;
    std::vector<std::map<std::string, std::string>> annotations;
    for (const auto& child: node->getChildren()) {
        std::string symbolName = child->getAnnotation(SYMBOL_NAME);
        if (child->getName() == "Terminal") {
            if (grammarBuilder->grammarBuilder.verifyTerminalExistence(symbolName)) {
                rule.push_back(grammarBuilder->grammarBuilder.getTerminal(symbolName));
            }
        } else if (child->getName() == "NonTerminal") {
            if (grammarBuilder->grammarBuilder.verifyNonTerminalExistence(symbolName)) {
                rule.push_back(grammarBuilder->grammarBuilder.getNonTerminal(symbolName));
            } else {
                std::shared_ptr<NonTerminal> rightSideNonTerminal = std::make_shared<NonTerminal>(symbolName);
                grammarBuilder->grammarBuilder.addNonTerminal(rightSideNonTerminal);
                rule.push_back(rightSideNonTerminal);
            }
        }

        auto grammarAnnotations = child->getAnnotations();
        cleanAnnotations(grammarAnnotations);
        annotations.push_back(grammarAnnotations);
    }

    grammarBuilder->grammarBuilder.addRule(nonTerminalName, rule, annotations);
    return true;
}

static bool descendAndAdd(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto grammarBuilder = dynamic_cast<GrammarBuilderVisitor*>(context);
    if (!grammarBuilder) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be GrammarBuilderVisitor.";
        throw std::runtime_error(what.str());
    }

    for (const auto& child: node->getChildren()) {
        grammarBuilder->visit(child);
    }

    std::shared_ptr<NonTerminal> beforeStart = std::make_shared<NonTerminal>(BEFORE_START_NAME);
    grammarBuilder->grammarBuilder.addNonTerminal(beforeStart);
    grammarBuilder->grammarBuilder.addTerminal(END_OF_INPUT);

    std::vector<std::shared_ptr<Symbol>> rule;
    std::shared_ptr<NonTerminal> start = grammarBuilder->grammarBuilder.getNonTerminal("Start");
    rule.push_back(start);
    rule.push_back(END_OF_INPUT);

    std::vector<std::map<std::string, std::string>> annotations(2, std::map<std::string, std::string>({}));

    grammarBuilder->grammarBuilder.addRule(BEFORE_START_NAME, rule, annotations);

    return true;
}

GrammarBuilderVisitor::GrammarBuilderVisitor() {
    this->setVisit("SStart", descend);
    this->setVisit("Start", descend);
    this->setVisit("Tokens", descend);
    this->setVisit("Token", visitToken);
    this->setVisit("Rules", descendAndAdd);
    this->setVisit("Rule", visitRule);

    this->setDefaultVisit(idle);
}
