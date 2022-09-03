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

    grammarBuilder->grammarVerifier.addTerminal(terminal);

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

    if (!grammarBuilder->grammarVerifier.verifyNonTerminalExistence(nonTerminalName)) {
        std::shared_ptr<NonTerminal> nonTerminal = std::make_shared<NonTerminal>(nonTerminalName);

        grammarBuilder->grammarVerifier.addNonTerminal(nonTerminal);
    }

    std::vector<std::shared_ptr<Symbol>> rule;
    std::vector<std::map<std::string, std::string>> annotations;
    for (const auto& child: node->getChildren()) {
        std::string symbolName = child->getAnnotation(SYMBOL_NAME);
        if (child->getName() == "Terminal") {
            if (grammarBuilder->grammarVerifier.verifyTerminalExistence(symbolName)) {
                rule.push_back(grammarBuilder->grammarVerifier.getTerminal(symbolName));
            }
        } else if (child->getName() == "NonTerminal") {
            if (grammarBuilder->grammarVerifier.verifyNonTerminalExistence(symbolName)) {
                rule.push_back(grammarBuilder->grammarVerifier.getNonTerminal(symbolName));
            } else {
                std::shared_ptr<NonTerminal> rightSideNonTerminal = std::make_shared<NonTerminal>(symbolName);
                grammarBuilder->grammarVerifier.addNonTerminal(rightSideNonTerminal);
                rule.push_back(rightSideNonTerminal);
            }
        }

        auto grammarAnnotations = child->getAnnotations();
        cleanAnnotations(grammarAnnotations);
        annotations.push_back(grammarAnnotations);
    }

    grammarBuilder->grammarVerifier.addRule(nonTerminalName, rule, annotations);

    return true;
}

GrammarBuilderVisitor::GrammarBuilderVisitor() {
    this->setVisit("SStart", descend);
    this->setVisit("Start", descend);
    this->setVisit("Tokens", descend);
    this->setVisit("Token", visitToken);
    this->setVisit("Rules", descend);
    this->setVisit("Rule", visitRule);

    this->setDefaultVisit(idle);
}
