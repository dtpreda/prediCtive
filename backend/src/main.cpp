#include <iostream>

#include "parser/prediCtiveParser.h"
#include "parser/visitors/grammar/GrammarBuilderVisitor.h"
#include "parser/grammar/GrammarBuilder.h"
#include "parser/visitors/astConversion/ParseTreeSimplifierVisitor.h"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Wrong number of arguments" << std::endl;
        return -1;
    }

    Parser prediCtiveParser = buildPrediCtiveParser();

    std::shared_ptr<Node> root = prediCtiveParser.openAndParse(argv[1]);

    convertToAST(root);

    GrammarBuilderVisitor gbv;

    gbv.visit(root);

    GrammarBuilder gv = gbv.grammarBuilder;
    gv.computeSets();

    Parser newGrammarParser = gv.buildGrammar();

    std::shared_ptr<Node> parseTree = newGrammarParser.openAndParse(argv[2]);

    ParseTreeSimplifierVisitor ptsv;

    ptsv.visit(parseTree);

    std::cout << parseTree->print() << std::endl;

    return 0;
}
