//
// Created by dtpreda on 28/07/22.
//

#include "Parser.h"
#include "parser/grammar/Symbol.h"
#include "parser/grammar/Terminal.h"
#include "parser/grammar/NonTerminal.h"
#include "parser/node/Node.h"
#include "utils.h"

#include <utility>
#include <fstream>
#include <sstream>
#include <streambuf>

Parser::Parser(Recognizer recognizer, const NonTerminal& startSymbol) : recognizer(std::move(recognizer)), startSymbol(startSymbol) {}

Parser::Parser(Recognizer recognizer, const NonTerminal &startSymbol, std::vector<Terminal> skipExpressions) :
    recognizer(std::move(recognizer)), startSymbol(startSymbol), skipExpressions(std::move(skipExpressions)) {}

std::shared_ptr<Node> Parser::parse(std::string toParse) {
    std::shared_ptr<Node> root = std::make_shared<Node>(Node(this->startSymbol.getName()));

    Terminal currentTerminal = (this->recognizer).recognizeFirstTerminal(toParse);

    std::vector<std::shared_ptr<Symbol>> nextSymbols = this->startSymbol.getRule(currentTerminal);
    std::vector<std::map<std::string, std::string>> annotations = this->startSymbol.getAnnotation(currentTerminal);

    for (int index = 0; index < nextSymbols.size(); index++) {
        if (currentTerminal.getName() == END_OF_INPUT->getName() && !nextSymbols[index]->isNullable()) {
            throw std::runtime_error("Unable to correctly match expression given.");
        }
        for (auto& annotation: annotations[index]) {
            if (annotation.second == LAST_TOKEN) {
                annotation.second = currentTerminal.getLastMatch();
            }
        }
        std::shared_ptr<Node> childNode = this->parse(
                toParse, currentTerminal,
                std::make_shared<Node>(Node(nextSymbols[index]->getName())), nextSymbols[index]
                );
        for (const auto& annotation: annotations[index]) {
            childNode->addAnnotation(annotation.first, annotation.second);
        }
        childNode->setParent(root);
        root->addChild(childNode);
    }

    if (!toParse.empty() || currentTerminal.getName() != END_OF_INPUT->getName()) {
        throw std::runtime_error("Unable to correctly match expression given.");
    }

    return root;
}

std::shared_ptr<Node> Parser::parse(std::string& toParse, Terminal& currentTerminal, const std::shared_ptr<Node>& rootNode, const std::shared_ptr<Symbol>& currentSymbol) {
    auto nextTerminal = std::dynamic_pointer_cast<const Terminal>(currentSymbol);
    auto nextNonTerminal = std::dynamic_pointer_cast<const NonTerminal>(currentSymbol);

    if (nextNonTerminal) {
        std::vector<std::shared_ptr<Symbol>> nextSymbols = nextNonTerminal->getRule(currentTerminal);
        std::vector<std::map<std::string, std::string>> annotations = nextNonTerminal->getAnnotation(currentTerminal);

        for (int index = 0; index < nextSymbols.size(); index++) {
            if (currentTerminal.getName() == END_OF_INPUT->getName() && !nextSymbols[index]->isNullable()) {
                throw std::runtime_error("Unable to correctly match expression given.");
            }
            for (auto& annotation: annotations[index]) {
                if (annotation.second == LAST_TOKEN) {
                    annotation.second = currentTerminal.getLastMatch();
                }
            }
            std::shared_ptr<Node> childNode = this->parse(
                    toParse, currentTerminal,
                    std::make_shared<Node>(Node(nextSymbols[index]->getName())), nextSymbols[index]
                    );
            for (const auto& annotation: annotations[index]) {
                childNode->addAnnotation(annotation.first, annotation.second);
            }
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

std::shared_ptr<Node> Parser::openAndParse(const std::string& path) {
    std::ifstream file(path);
    std::string fileStr((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    std::stringstream filteredFile;

    std::regex quoteExpression("\"(.*?)\"");

    while (!fileStr.empty()) {
        std::smatch matchResult;

        if (std::regex_search(fileStr, matchResult, quoteExpression) && matchResult.position() == 0) {
            filteredFile << fileStr.substr(0, matchResult.length());

            fileStr = fileStr.substr(matchResult.length());
        } else {
            bool hasMatch = false;
            for (const auto& skipExpression: this->skipExpressions) {
                std::regex skipExpressionRegex(skipExpression.getRegexExpression());

                if (std::regex_search(fileStr, matchResult, skipExpressionRegex) && matchResult.position() == 0) {
                    fileStr = fileStr.substr(matchResult.length());
                    hasMatch = true;
                    break;
                }
            }

            if (!hasMatch) {
                filteredFile << fileStr[0];
                fileStr = fileStr.substr(1);
            }
        }
    }

    std::string str = filteredFile.str();

    return parse(filteredFile.str());
}


