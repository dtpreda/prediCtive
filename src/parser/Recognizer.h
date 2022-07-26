//
// Created by dtpreda on 26/07/22.
//

#ifndef PARSER_RECOGNIZER_H
#define PARSER_RECOGNIZER_H

#include <vector>

#include "Terminal.h"

class Recognizer {
public:
    Recognizer() = default;

    void addTerminal(const Terminal& terminal);
    Terminal recognizeFirstTerminal(std::string& toRecognize);
private:
    std::vector<std::pair<std::regex, Terminal>> terminals;
};


#endif //PARSER_RECOGNIZER_H
