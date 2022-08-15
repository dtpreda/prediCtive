//
// Created by dtpreda on 15/08/22.
//

#ifndef PARSER_SKIPEXTRACTORVISITOR_H
#define PARSER_SKIPEXTRACTORVISITOR_H

#include "Visitor.h"

#include <vector>
#include <string>

class SkipExtractorVisitor : public Visitor<bool> {
public:
    SkipExtractorVisitor();
    ~SkipExtractorVisitor() override = default;

    void addRegexExpression(const std::string& regexExpression);
    std::vector<std::string> getRegexExpressions() const;
private:
    std::vector<std::string> regexExpressions;
};


#endif //PARSER_SKIPEXTRACTORVISITOR_H
