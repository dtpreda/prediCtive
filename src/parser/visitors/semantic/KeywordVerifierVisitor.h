//
// Created by dtpreda on 30/08/22.
//

#ifndef PARSER_KEYWORDVERIFIERVISITOR_H
#define PARSER_KEYWORDVERIFIERVISITOR_H

#include "parser/visitors/Visitor.h"

class KeywordVerifierVisitor : public Visitor<bool> {
public:
    KeywordVerifierVisitor();
    ~KeywordVerifierVisitor() override = default;
};


#endif //PARSER_KEYWORDVERIFIERVISITOR_H
