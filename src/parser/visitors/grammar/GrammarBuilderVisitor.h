//
// Created by dtpreda on 02/09/22.
//

#ifndef PARSER_GRAMMARBUILDERVISITOR_H
#define PARSER_GRAMMARBUILDERVISITOR_H

#include "parser/visitors/Visitor.h"
#include "parser/grammar/GrammarBuilder.h"

class GrammarBuilderVisitor : public Visitor<bool>{
public:
    GrammarBuilderVisitor();
    ~GrammarBuilderVisitor() override = default;

    GrammarBuilder grammarBuilder;
};


#endif //PARSER_GRAMMARBUILDERVISITOR_H
