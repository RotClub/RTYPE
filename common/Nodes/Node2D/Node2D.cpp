/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Node2D
*/

#include "Node2D.hpp"

#include <Engine.hpp>

Node2D::Node2D(const std::string &name) : Node(name), position(Types::Vector2::Zero())
{
    this->metatable = "Node2DMetaTable";
}

void Node2D::addChild(Node &child)
{
    Node::addChild(child);
    _updateGlobalPosition();
}

void Node2D::Update() { _updateGlobalPosition(); }


void Node2D::_updateGlobalPosition()
{
    Node *parentNode = getParent();
    if (parentNode == nullptr) {
        _globalPosition = position;
        return;
    }
    if (Node2D *node2D = dynamic_cast<Node2D *>(parentNode)) {
        _globalPosition = node2D->getGlobalPosition() + position;
        return;
    }
    _globalPosition = position;
}
