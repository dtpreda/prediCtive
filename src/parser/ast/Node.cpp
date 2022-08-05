//
// Created by dtpreda on 31/07/22.
//

#include "Node.h"

#include <stdexcept>
#include <utility>
#include <sstream>

Node::Node(std::string name) : name(std::move(name)) {}

void Node::addChild(const std::shared_ptr<Node>& child) {
    this->children.push_back(std::make_shared<Node>(*child));
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
            this->children.push_back(std::make_unique<Node>(*child));
        }
    }
    return *this;
}

Node::Node(const Node& other) {
    this->name = other.name;

    this->children.clear();
    for (auto& child : other.children) {
        this->children.push_back(std::make_shared<Node>(*child));
    }

    this->parent = other.parent;
}

std::string Node::getName() const {
    return this->name;
}

void Node::setParent(const std::shared_ptr<Node>& parentPtr) {
    std::weak_ptr<Node> as = std::weak_ptr<Node>(parentPtr);
    this->parent = std::weak_ptr<Node>(parentPtr);
}

Node Node::getParent() const {
    return *(this->parent.lock());
}

void Node::addAnnotation(const std::string& key, const std::string& annotation) {
    if (this->annotations.find(key) != this->annotations.end()) {
        std::stringstream what;
        what << "Node already contains an annotation with a " << key << " key.";
        throw std::runtime_error(what.str());
    }

    this->annotations.insert({key, annotation});
}

std::string Node::getAnnotation(const std::string& key) const {
    if (this->annotations.find(key) == this->annotations.end()) {
        std::stringstream what;
        what << "Node does not contain an annotation with a " << key << " key.";
        throw std::runtime_error(what.str());
    }

    return this->annotations.at(key);
}
