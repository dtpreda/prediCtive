//
// Created by dtpreda on 31/07/22.
//

#ifndef PARSER_NODE_H
#define PARSER_NODE_H

#include <vector>

#include "Symbol.h"

class Node {
public:
    explicit Node(std::string name);
    Node(const Node& other);
    ~Node();

    std::string getName() const;

    void addChild(const Node& child);
    Node getChild(int index) const;

    Node& operator=(const Node& other);
private:
    std::string name;
    std::vector<Node*> children;
};


#endif //PARSER_NODE_H
