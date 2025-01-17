//
// Created by Yanis Moyon on 14/01/2025.
//

#include "StaticBody2D.hpp"

#include <Nodes/Shape2D/Rectangle2D/Rectangle2D.hpp>

StaticBody2D::StaticBody2D(const std::string &name, const Types::Vector2 &position, const Types::Vector2 &size) : Body2D(name)
{
    this->metatable = "StaticBody2DMetaTable";
    this->velocity = Types::Vector2(0, 0);
    CollisionShape2D *shape = new CollisionShape2D(name + "_shape");
    shape->setShape(new Rectangle2D(position, size));
    Node2D::addChild(*shape);
}
