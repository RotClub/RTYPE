/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Node
*/

#include "Node.hpp"

#include <Engine.hpp>
#include <raylib-cpp.hpp>

Node::Node(const std::string &name) : children(std::vector<Node *>()), name(name)
{
    if (Engine::isInstanceStarted()) {
        Engine &engine = Engine::GetInstance();
        if (engine.root != nullptr && name == "root") {
            throw std::runtime_error("Root node already exists");
        }
    }
    this->metatable = "NodeMetaTable";
}

Node *Node::GetChild(const std::string &name) const
{
    for (auto child : children) {
        if (child->name == name)
            return child;
    }
    throw std::runtime_error("Node not found");
}

std::vector<Node *> Node::GetChildren() const { return children; }

void Node::addChild(Node &child)
{
    children.push_back(&child);
    child._parent = this;
}

void Node::Destroy()
{
    if (this->name == "root") {
        throw std::runtime_error("Cannot destroy root node");
    }
    if (_parent != nullptr) {
        std::erase(_parent->children, this);
    }
    for (auto child : children) {
        child->Destroy();
    }
    children.clear();
    delete this;
}
