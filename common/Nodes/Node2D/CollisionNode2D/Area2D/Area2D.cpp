/*
** EPITECH PROJECT, 2025
** RTYPE
** File description:
** Area2D
*/

#include "Area2D.hpp"
#include "Nodes/Node2D/CollisionNode2D/CollisionNode2D.hpp"
#include "Nodes/Node2D/CollisionShape2D/CollisionShape2D.hpp"
#include "Nodes/Shape2D/Rectangle2D/Rectangle2D.hpp"

Area2D::Area2D(const std::string &name, Types::Vector2 position, Types::Vector2 size)
    : CollisionNode2D(name)
{
    this->metatable = "Area2DMetaTable";
    this->position = position;
    _size = size;
    Rectangle2D* rectangle = new Rectangle2D(position, size);
    CollisionShape2D* area2dCollision = new CollisionShape2D("area2d_collision");
    area2dCollision->setShape(rectangle);
    CollisionNode2D::addCollisionShape(area2dCollision);
}

Types::Vector2 Area2D::getSize() const
{
    return _size;
}

void Area2D::setSize(const Types::Vector2 &size)
{
    _size = size;
    CollisionShape2D* area2dCollision = CollisionNode2D::getCollisionShape("area2d_collision");
    Rectangle2D* rectangle = new Rectangle2D(position, _size);
    area2dCollision->setShape(rectangle);
    CollisionNode2D::removeCollisionShape("area2d_collision");
    CollisionNode2D::addCollisionShape(area2dCollision);
}
