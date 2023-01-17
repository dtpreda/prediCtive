//
// Created by dtpreda on 04/09/22.
//

#include "ParseTreeSimplifierVisitor.h"

#include <sstream>
#include <stdexcept>
#include <regex>

#include "parser/utils.h"

static bool descend(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    for (auto& child : node->getChildren()) {
        context->visit(child);
    }

    auto parseTreeSimplifier = dynamic_cast<ParseTreeSimplifierVisitor*>(context);
    if (!parseTreeSimplifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be ParseTreeSimplifierVisitor.";
        throw std::runtime_error(what.str());
    }

    return true;
}

static bool descendAndFix(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto parseTreeSimplifier = dynamic_cast<ParseTreeSimplifierVisitor*>(context);
    if (!parseTreeSimplifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be ParseTreeSimplifierVisitor.";
        throw std::runtime_error(what.str());
    }

    std::vector<std::shared_ptr<Node>> children = node->getChildren();
    for (auto& child: children) {
        parseTreeSimplifier->visit(child);
    }

    std::regex intermediateNodeRegex(INTERMEDIATE_NON_TERMINAL_PATTERN);
    std::smatch matchResult;
    std::string nodeName = node->getName();

    if (std::regex_search(nodeName, matchResult, intermediateNodeRegex) && matchResult.position() == 0) {
        std::shared_ptr<Node> grandParent = node->getParent();
        grandParent->replaceChild(node, node->getChildren());

        for (const auto& annotation: node->getAnnotations()) {
            try {
                grandParent->addAnnotation(annotation.first, annotation.second);
            } catch (std::runtime_error& e) {
                grandParent->deleteAnnotation(annotation.first);
                grandParent->addAnnotation(annotation.first, annotation.second);
            }
        }
    }


    return true;
}

ParseTreeSimplifierVisitor::ParseTreeSimplifierVisitor() {
    this->setVisit("Start", descend);

    this->setDefaultVisit(descendAndFix);
}
