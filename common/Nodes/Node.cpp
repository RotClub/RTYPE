/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Node
*/

#include "Node.hpp"
#include <raylib-cpp.hpp>

Node::Node(const std::string &name)
    : children(std::vector<Node*>()), name(name)
{
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

std::vector<Node *> Node::GetChildren() const
{
    return children;
}

void Node::addChild(Node &child)
{
    children.push_back(&child);
    child._parent = this;
}

void Node::Update()
{
}

void Node::Draw()
{
}
