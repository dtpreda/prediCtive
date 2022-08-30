//
// Created by dtpreda on 05/08/22.
//

#include "Visitor.h"

#include <stdexcept>
#include <sstream>

template<class T>
T Visitor<T>::visit(std::shared_ptr<Node> node) {
    if (this->methods.find(node->getName()) == this->methods.end()) {
        if (this->defaultVisit != nullptr) {
            return this->defaultVisit(this, node);
        } else {
            std::stringstream what;
            what << "No visit method available for node of type " << node->getName() << ".";
            throw std::runtime_error(what.str());
        }
    }

    return this->methods.find(node->getName())->second(this, node);
}

template<class T>
void Visitor<T>::setVisit(std::string nodeName, std::function<T(Visitor<T>*, std::shared_ptr<Node>)> method) {
    if (this->methods.find(nodeName) != this->methods.end()) {
        std::stringstream what;
        what << "There is already a method set for nodes of type" << nodeName << ".";
        throw std::runtime_error(what.str());
    }

    this->methods.insert({nodeName, method});
}

template<class T>
void Visitor<T>::setDefaultVisit(std::function<T(Visitor<T>*, std::shared_ptr<Node>)> defaultVisit) {
    this->defaultVisit = defaultVisit;
}
