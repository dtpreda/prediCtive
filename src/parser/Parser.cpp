//
// Created by dtpreda on 28/07/22.
//

#include "Parser.h"
#include "Symbol.h"
#include "Terminal.h"
#include "NonTerminal.h"
#include "Node.h"
#include "utils.h"

#include <utility>

Parser::Parser(Recognizer recognizer, NonTerminal startSymbol) : recognizer(std::move(recognizer)), startSymbol(std::move(startSymbol)){}

Node Parser::parse(std::string toParse) const {
    Node root = Node(this->startSymbol.getName());

    Terminal currentTerminal = (this->recognizer).recognizeFirstTerminal(toParse);

    std::vector<std::shared_ptr<Symbol>> nextSymbols = this->startSymbol.getRule(currentTerminal);

    for (auto & nextSymbol : nextSymbols) {
        if (currentTerminal.getName() == END_OF_INPUT->getName() && nextSymbol->getName() != END_OF_INPUT->getName()) {
            throw std::runtime_error("Unable to correctly match expression given.");
        }
        Node childNode = this->parse(toParse, currentTerminal, Node(nextSymbol->getName()), nextSymbol);
        root.addChild(childNode);
    }

    if (!toParse.empty() || currentTerminal.getName() != END_OF_INPUT->getName()) {
        throw std::runtime_error("Unable to correctly match expression given.");
    }

    return root;
}

Node Parser::parse(std::string& toParse, Terminal& currentTerminal, Node rootNode, const std::shared_ptr<Symbol>& currentSymbol) const {
    auto nextTerminal = std::dynamic_pointer_cast<const Terminal>(currentSymbol);
    auto nextNonTerminal = std::dynamic_pointer_cast<const NonTerminal>(currentSymbol);

    if (nextNonTerminal) {
        std::vector<std::shared_ptr<Symbol>> nextSymbols = nextNonTerminal->getRule(currentTerminal);

        for (auto & nextSymbol : nextSymbols) {
            Node childNode = this->parse(toParse, currentTerminal, Node(nextSymbol->getName()), nextSymbol);
            rootNode.addChild(childNode);
        }
    } else if (nextTerminal) {
        if (nextTerminal->getName() != currentTerminal.getName()) {
            throw std::runtime_error("Unable to correctly match expression given.");
        }

        rootNode.addChild(Node(nextTerminal->getName()));

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


