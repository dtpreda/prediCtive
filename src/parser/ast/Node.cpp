//
// Created by dtpreda on 31/07/22.
//

#include "Node.h"

#include <stdexcept>
#include <utility>
#include <sstream>

Node::Node(std::string name) : name(std::move(name)) {}

void Node::addChild(const std::shared_ptr<Node>& child) {
    this->children.push_back(child);
}

std::shared_ptr<Node> Node::getChild(int index) const {
    if (this->children.size() <= index) {
        throw std::runtime_error("No such child index.");
    }

    return this->children.at(index);
}

Node &Node::operator=(const Node &other) {
    if (this != &other) {
        this->name = other.name;

        this->children.clear();
        for (auto& child : other.children) {
            this->children.push_back(std::make_shared<Node>(*child));
        }

        this->annotations.clear();
        for (auto& annotation : other.annotations) {
            this->annotations.insert(annotation);
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

    this->annotations.clear();
    for (auto& annotation : other.annotations) {
        this->annotations.insert({annotation.first, annotation.second});
    }
}

std::string Node::getName() const {
    return this->name;
}

void Node::setParent(const std::shared_ptr<Node>& parentPtr) {
    this->parent = parentPtr;
}

std::shared_ptr<Node> Node::getParent() const {
    return this->parent;
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

std::vector<std::shared_ptr<Node>> Node::getChildren() const {
    return this->children;
}

void Node::clearChildren() {
    this->children.clear();
}

void Node::setName(const std::string& name) {
    this->name = name;
}

void Node::changeAnnotationKey(const std::string &originalKey, const std::string &newKey) {
    if (this->annotations.find(originalKey) == this->annotations.end()) {
        std::stringstream what;
        what << "Node does not contain an annotation with a " << originalKey << " key.";
        throw std::runtime_error(what.str());
    }

    std::string value = this->annotations.at(originalKey);
    this->annotations.erase(originalKey);
    this->addAnnotation(newKey, value);
}
