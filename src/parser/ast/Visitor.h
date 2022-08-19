//
// Created by dtpreda on 05/08/22.
//

#ifndef PARSER_VISITOR_H
#define PARSER_VISITOR_H

#include <map>
#include <functional>
#include <string>

#include "Node.h"

template <class T>
class Visitor {
public:
    T visit(std::shared_ptr<Node> node);
    virtual ~Visitor() = default;

    void setVisit(std::string nodeName, std::function<T(Visitor<T>*, std::shared_ptr<Node>)> method);
    void setDefaultVisit(std::function<T(Visitor<T>*, std::shared_ptr<Node>)> defaultVisit);
private:
    std::map<std::string, std::function<T(Visitor<T>*, std::shared_ptr<Node>)>> methods;
    std::function<T(Visitor<T>*, std::shared_ptr<Node>)> defaultVisit;
};

template class Visitor<bool>;
template class Visitor<std::string>;


#endif //PARSER_VISITOR_H
