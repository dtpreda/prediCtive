//
// Created by dtpreda on 27/07/22.
//

#ifndef PARSER_PREDICTIVEPARSER_H
#define PARSER_PREDICTIVEPARSER_H

#include "Parser.h"
#include "visitors/astConversion/TokenExtractorVisitor.h"
#include "visitors/astConversion/SkipExtractorVisitor.h"
#include "visitors/astConversion/RuleExtractorVisitor.h"
#include "visitors/astConversion/RuleSimplifierVisitor.h"
#include "visitors/astConversion/ClosureSimplifierVisitor.h"

Parser buildPrediCtiveParser();
void convertToAST(const std::shared_ptr<Node>& root);

#endif //PARSER_PREDICTIVEPARSER_H
