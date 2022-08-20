//
// Created by dtpreda on 31/07/22.
//

#ifndef PARSER_NODE_H
#define PARSER_NODE_H

#include <vector>
#include <memory>
#include <map>
#include <string>

#include "parser/grammar/Symbol.h"

class Node {
public:
    explicit Node(std::string name);
    Node(const Node& other);

    std::string getName() const;
    void setName(const std::string& name);

    void addChild(const std::shared_ptr<Node>& child);
    std::shared_ptr<Node> getChild(int index) const;
    void replaceChild(const std::shared_ptr<Node>& childToReplace, const std::vector<std::shared_ptr<Node>>& newChildren);
    std::vector<std::shared_ptr<Node>> getChildren() const;
    void clearChildren();

    void setParent(const std::shared_ptr<Node>& parent);
    std::shared_ptr<Node> getParent() const;

    void addAnnotation(const std::string& key, const std::string& annotation);
    std::string getAnnotation(const std::string& key) const;
    void changeAnnotationKey(const std::string& originalKey, const std::string& newKey);
    void deleteAnnotation(const std::string& key);
    void clearAnnotations();

    Node& operator=(const Node& other);
private:
    std::string name;
    std::vector<std::shared_ptr<Node>> children;
    std::shared_ptr<Node> parent;
    std::map<std::string, std::string> annotations;
};


#endif //PARSER_NODE_H
