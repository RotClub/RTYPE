/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Node
*/

#include "Node.hpp"

Node::Node(const std::string &name)
    : children(), name("")
{
}

Node::~Node()
{
}

const Node &Node::GetChild(const std::string &name) const
{
    for (const auto &child : children) {
        if (child.name == name)
            return child;
    }
    throw std::runtime_error("Node not found");
}

const std::vector<Node> &Node::GetChildren() const
{
    return children;
}
