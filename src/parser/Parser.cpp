//
// Created by dtpreda on 28/07/22.
//

#include "Parser.h"
#include "parser/grammar/Symbol.h"
#include "parser/grammar/Terminal.h"
#include "parser/grammar/NonTerminal.h"
#include "parser/ast/Node.h"
#include "utils.h"

#include <utility>

Parser::Parser(Recognizer recognizer, const NonTerminal& startSymbol) : recognizer(std::move(recognizer)), startSymbol(startSymbol) {}

Node Parser::parse(std::string toParse) {
    std::shared_ptr<Node> root = std::make_shared<Node>(Node(this->startSymbol.getName()));

    Terminal currentTerminal = (this->recognizer).recognizeFirstTerminal(toParse);

    std::vector<std::shared_ptr<Symbol>> nextSymbols = this->startSymbol.getRule(currentTerminal);

    for (auto & nextSymbol : nextSymbols) {
        if (currentTerminal.getName() == END_OF_INPUT->getName() && !nextSymbol->isNullable()) {
            throw std::runtime_error("Unable to correctly match expression given.");
        }
        std::shared_ptr<Node> childNode = this->parse(
                toParse, currentTerminal,
                std::make_shared<Node>(Node(nextSymbol->getName())), nextSymbol
                );
        childNode->setParent(root);
        root->addChild(childNode);
    }

    if (!toParse.empty() || currentTerminal.getName() != END_OF_INPUT->getName()) {
        throw std::runtime_error("Unable to correctly match expression given.");
    }

    return *root;
}

std::shared_ptr<Node> Parser::parse(std::string& toParse, Terminal& currentTerminal, const std::shared_ptr<Node>& rootNode, const std::shared_ptr<Symbol>& currentSymbol) {
    auto nextTerminal = std::dynamic_pointer_cast<const Terminal>(currentSymbol);
    auto nextNonTerminal = std::dynamic_pointer_cast<const NonTerminal>(currentSymbol);

    if (nextNonTerminal) {
        std::vector<std::shared_ptr<Symbol>> nextSymbols = nextNonTerminal->getRule(currentTerminal);

        for (auto & nextSymbol : nextSymbols) {
            if (currentTerminal.getName() == END_OF_INPUT->getName() && !nextSymbol->isNullable()) {
                throw std::runtime_error("Unable to correctly match expression given.");
            }
            std::shared_ptr<Node> childNode = this->parse(
                    toParse, currentTerminal,
                    std::make_shared<Node>(Node(nextSymbol->getName())), nextSymbol
                    );
            childNode->setParent(rootNode);
            rootNode->addChild(childNode);
        }
    } else if (nextTerminal) {
        if (nextTerminal->getName() != currentTerminal.getName()) {
            throw std::runtime_error("Unable to correctly match expression given.");
        }

        rootNode->addAnnotation("consumed_token", currentTerminal.getLastMatch());

        try {
            currentTerminal = (this->recognizer).recognizeFirstTerminal(toParse);
        } catch (std::runtime_error& e) {
            if (toParse.empty()) {
                currentTerminal = *END_OF_INPUT;
                return rootNode;
            } else {
                throw e;
            }
        }
    } else {
        throw std::runtime_error("Stack was corrupted during input validation.");
    }

    return rootNode;
}


