//
// Created by dtpreda on 04/08/22.
//

#include "TestUtils.h"

#include <fstream>
#include <sstream>

std::string TestUtils::openPrediCtiveFile(const std::string& path) {
    std::ifstream file(path);
    std::stringstream filteredFile;

    char c;

    while (file >> c) {
        if (c != '\n' && c != '\t' && c != ' ') {
            filteredFile << c;
        }
    }

    file.close();

    return filteredFile.str();
}
