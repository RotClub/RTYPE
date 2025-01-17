//
// Created by Yanis Moyon on 13/01/2025.
//

#include "RigidBody2D.hpp"

#include <Nodes/Shape2D/Rectangle2D/Rectangle2D.hpp>

RigidBody2D::RigidBody2D(const std::string &name, const Types::Vector2 &position, const Types::Vector2 &size, const Types::Vector2 &velocity) :
    Body2D(name)
{
    this->metatable = "RigidBody2DMetaTable";
    this->velocity = velocity;
    CollisionShape2D *shape = new CollisionShape2D(name + "_shape");
    shape->setShape(new Rectangle2D(position, size));
    Node2D::addChild(*shape);
}
