//
// Created by dtpreda on 04/09/22.
//

#ifndef PARSER_PARSETREESIMPLIFIERVISITOR_H
#define PARSER_PARSETREESIMPLIFIERVISITOR_H


#include "parser/visitors/Visitor.h"

class ParseTreeSimplifierVisitor : public Visitor<bool> {
public:
    ParseTreeSimplifierVisitor();
    ~ParseTreeSimplifierVisitor() override = default;
};


#endif //PARSER_PARSETREESIMPLIFIERVISITOR_H
