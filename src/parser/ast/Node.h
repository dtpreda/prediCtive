//
// Created by dtpreda on 31/07/22.
//

#ifndef PARSER_NODE_H
#define PARSER_NODE_H

#include <vector>
#include <memory>

#include "parser/grammar/Symbol.h"

class Node {
public:
    explicit Node(std::string name);
    Node(const Node& other);

    std::string getName() const;

    void addChild(const std::shared_ptr<Node>& child);
    Node getChild(int index) const;

    void setParent(const std::shared_ptr<Node>& parent);
    Node getParent() const;

    Node& operator=(const Node& other);
private:
    std::string name;
    std::vector<std::shared_ptr<Node>> children;
    std::weak_ptr<Node> parent;
};


#endif //PARSER_NODE_H
