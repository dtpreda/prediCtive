//
// Created by dtpreda on 30/08/22.
//

#include "KeywordVerifierVisitor.h"
#include "parser/utils.h"

#include <sstream>
#include <stdexcept>
#include <regex>

static bool idle(Visitor<bool>* context, const std::shared_ptr<Node>& root) {
    return true;
}

static bool descend(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    for (auto& child : node->getChildren()) {
        context->visit(child);
    }

    return true;
}

static bool visitToken(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto keywordVerifier = dynamic_cast<KeywordVerifierVisitor*>(context);
    if (!keywordVerifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be KeywordVerifierVisitor.";
        throw std::runtime_error(what.str());
    }

    std::string identifier = node->getChild(0)->getAnnotation(CONSUMED_TOKEN);
    if (KEYWORDS.find(identifier) != KEYWORDS.end()) {
        std::stringstream what;
        what << identifier << " is a prediCtive keyword. It should not be used as a Symbol name." << std::endl;
        throw std::runtime_error(what.str());
    }

    std::regex intermediatePattern(INTERMEDIATE_NON_TERMINAL_PATTERN);
    std::smatch matchResult;
    if (std::regex_search(identifier, matchResult, intermediatePattern)) {
        std::stringstream what;
        what << INTERMEDIATE_NON_TERMINAL_PATTERN << "[any number]" << " is a Symbol name pattern reserved by prediCtive. It should not be used as a Symbol name." << std::endl;
        throw std::runtime_error(what.str());
    }

    try {
        std::regex tokenExpression(node->getChild(2)->getAnnotation(CONSUMED_TOKEN));
    } catch (std::regex_error& e) {
        std::stringstream what;
        what << node->getChild(2)->getAnnotation(CONSUMED_TOKEN) << " is not a valid ECMAScript regular expression pattern." << std::endl;
        throw std::runtime_error(what.str());
    }

    return true;
}

static bool visitNextToken(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto keywordVerifier = dynamic_cast<KeywordVerifierVisitor*>(context);
    if (!keywordVerifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be KeywordVerifierVisitor.";
        throw std::runtime_error(what.str());
    }

    if (!node->getChildren().empty()) {
        keywordVerifier->visit(node->getChild(1));
        keywordVerifier->visit(node->getChild(2));
    }

    return true;
}

static bool visitSkip(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto keywordVerifier = dynamic_cast<KeywordVerifierVisitor*>(context);
    if (!keywordVerifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be KeywordVerifierVisitor.";
        throw std::runtime_error(what.str());
    }

    if (!node->getChildren().empty()) {
        try {
            std::regex tokenExpression(node->getChild(2)->getAnnotation(CONSUMED_TOKEN));
        } catch (std::regex_error& e) {
            std::stringstream what;
            what << node->getChild(2)->getAnnotation(CONSUMED_TOKEN) << " is not a valid ECMAScript regular expression pattern." << std::endl;
            throw std::runtime_error(what.str());
        }
        keywordVerifier->visit(node->getChild(3));
    }

    return true;
}

static bool visitSkipExpression(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto keywordVerifier = dynamic_cast<KeywordVerifierVisitor*>(context);
    if (!keywordVerifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be KeywordVerifierVisitor.";
        throw std::runtime_error(what.str());
    }

    if (!node->getChildren().empty()) {
        try {
            std::regex tokenExpression(node->getChild(1)->getAnnotation(CONSUMED_TOKEN));
        } catch (std::regex_error& e) {
            std::stringstream what;
            what << node->getChild(1)->getAnnotation(CONSUMED_TOKEN) << " is not a valid ECMAScript regular expression pattern." << std::endl;
            throw std::runtime_error(what.str());
        }
        keywordVerifier->visit(node->getChild(2));
    }

    return true;
}

static bool visitRules(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto keywordVerifier = dynamic_cast<KeywordVerifierVisitor*>(context);
    if (!keywordVerifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be KeywordVerifierVisitor.";
        throw std::runtime_error(what.str());
    }

    if (node->getChildren().empty()) {
        return true;
    }

    std::string identifier = node->getChild(0)->getAnnotation(CONSUMED_TOKEN);

    if (KEYWORDS.find(identifier) != KEYWORDS.end()) {
        std::stringstream what;
        what << identifier << " is a prediCtive keyword. It should not be used as a Symbol name." << std::endl;
        throw std::runtime_error(what.str());
    }

    std::regex intermediatePattern(INTERMEDIATE_NON_TERMINAL_PATTERN);
    std::smatch matchResult;
    if (std::regex_search(identifier, matchResult, intermediatePattern)) {
        std::stringstream what;
        what << INTERMEDIATE_NON_TERMINAL_PATTERN << "[any number]" << " is a Symbol name pattern reserved by prediCtive. It should not be used as a Symbol name." << std::endl;
        throw std::runtime_error(what.str());
    }

    keywordVerifier->visit(node->getChild(4));
    keywordVerifier->visit(node->getChild(2));

    return true;
}

static bool visitAnnotation(Visitor<bool>* context, const std::shared_ptr<Node>& node) {
    auto keywordVerifier = dynamic_cast<KeywordVerifierVisitor*>(context);
    if (!keywordVerifier) {
        std::stringstream what;
        what << "Wrong derived class of Visitor<bool>. Should be KeywordVerifierVisitor.";
        throw std::runtime_error(what.str());
    }

    if (node->getChildren().empty()) {
        return true;
    }

    if (KEYWORDS.find(node->getChild(1)->getAnnotation(CONSUMED_TOKEN)) != KEYWORDS.end()) {
        std::stringstream what;
        what << node->getChild(1)->getAnnotation(CONSUMED_TOKEN) << " is a prediCtive keyword. It should not be used as an annotation key." << std::endl;
        throw std::runtime_error(what.str());
    }

    return true;
}

KeywordVerifierVisitor::KeywordVerifierVisitor() {
    this->setVisit("SStart", descend);
    this->setVisit("Start", descend);
    this->setVisit("Tokens", descend);
    this->setVisit("Token", visitToken);
    this->setVisit("NextToken", visitNextToken);
    this->setVisit("Skip", visitSkip);
    this->setVisit("SkipExpression", visitSkipExpression);
    this->setVisit("Rules", visitRules);
    this->setVisit("NextRule", visitRules);
    this->setVisit("Expansion", descend);
    this->setVisit("NextExpansionBlock", descend);
    this->setVisit("Annotation", visitAnnotation);

    this->setDefaultVisit(idle);
}
