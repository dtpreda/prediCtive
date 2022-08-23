//
// Created by dtpreda on 27/07/22.
//

#ifndef PARSER_PREDICTIVEPARSER_H
#define PARSER_PREDICTIVEPARSER_H

#include "Parser.h"
#include "ast/TokenExtractorVisitor.h"
#include "ast/SkipExtractorVisitor.h"
#include "ast/RuleExtractorVisitor.h"
#include "ast/RuleSimplifierVisitor.h"
#include "ast/ClosureSimplifierVisitor.h"

Parser buildPrediCtiveParser();
void convertToAST(const std::shared_ptr<Node>& root);

#endif //PARSER_PREDICTIVEPARSER_H
