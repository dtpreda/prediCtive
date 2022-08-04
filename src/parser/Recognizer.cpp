//
// Created by dtpreda on 26/07/22.
//

#include <regex>
#include <sstream>

#include "Recognizer.h"

void Recognizer::addTerminal(const Terminal& terminal) {
    std::stringstream terminalRegexExpression;
    terminalRegexExpression << "^" << terminal.getRegexExpression();

    std::regex terminalRegex(terminalRegexExpression.str());

    std::pair<std::regex, Terminal> toInsert(terminalRegex, terminal);
    this->terminals.push_back(toInsert);
}

Terminal Recognizer::recognizeFirstTerminal(std::string &toRecognize) const {
    std::smatch matchResult;
    for (auto & terminal : this->terminals) {
        if (std::regex_search(toRecognize, matchResult, terminal.first) && matchResult.position() == 0) {

            toRecognize = toRecognize.substr(matchResult.length());
            return terminal.second;
        }
    }

    throw std::runtime_error("No matching terminal was found for the given input.");
}

Recognizer::Recognizer(const std::vector<std::shared_ptr<Terminal>>& terminals) {
    for (auto& terminal : terminals) {
        this->addTerminal(*terminal);
    }
}
