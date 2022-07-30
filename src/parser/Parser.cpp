//
// Created by dtpreda on 28/07/22.
//

#include "Parser.h"
#include "Symbol.h"
#include "Terminal.h"
#include "NonTerminal.h"

#include <utility>
#include <stack>

Parser::Parser(Recognizer recognizer, const NonTerminal& startSymbol) : recognizer(std::move(recognizer)), startSymbol(startSymbol){}

bool Parser::parse(std::string toParse) const {
    std::stack<const Symbol*> validationStack;
    validationStack.push(&(this->startSymbol));

    Terminal currentTerminal = (this->recognizer).recognizeFirstTerminal(toParse);

    while (!validationStack.empty()) {
        const Symbol* stackTop = validationStack.top();
        validationStack.pop();

        auto* stackTopTerminal = dynamic_cast<const Terminal*>(stackTop);

        if (!stackTopTerminal) {
            auto* stackTopNonTerminal = dynamic_cast<const NonTerminal*>(stackTop);

            if (!stackTopNonTerminal) {
                throw std::runtime_error("Stack was corrupted during input validation.");
            }

            std::vector<Symbol*> nextSymbols = stackTopNonTerminal->getRule(currentTerminal);

            for (auto it = nextSymbols.rbegin(); it != nextSymbols.rend(); it++) {
                validationStack.push(*it);
            }
        } else {
            if (stackTopTerminal->getName() != currentTerminal.getName()) {
                return false;
            }

            if (toParse.empty()) {
                break;
            }

            currentTerminal = this->recognizer.recognizeFirstTerminal(toParse);
        }
    }

    if (validationStack.empty() && toParse.empty()) {
        return true;
    }

    return false;
}
