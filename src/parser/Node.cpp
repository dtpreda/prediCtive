//
// Created by dtpreda on 31/07/22.
//

#include "Node.h"

#include <stdexcept>
#include <utility>

Node::Node(std::string name) : name(std::move(name)) {}

void Node::addChild(const Node& child) {
    this->children.push_back(new Node(child));
}

Node Node::getChild(int index) const {
    if (this->children.size() <= index) {
        throw std::runtime_error("No such child index.");
    }

    return *(this->children.at(index));
}

Node &Node::operator=(const Node &other) {
    if (this != &other) {
        this->name = other.name;

        this->children.clear();
        for (auto& child : other.children) {
            this->children.push_back(new Node(*child));
        }
    }
    return *this;
}

Node::~Node() {
    for (auto& child: this->children) {
        delete child;
        child = nullptr;
    }
}

Node::Node(const Node& other) {
    this->name = other.name;

    this->children.clear();
    for (auto& child : other.children) {
        this->children.push_back(new Node(*child));
    }
}
