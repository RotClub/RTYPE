//
// Created by Yanis Moyon on 18/12/2024.
//

#include "CollisionShape2D.hpp"
#include "spdlog/spdlog.h"


CollisionShape2D::CollisionShape2D(const std::string &name) : shape(nullptr), collision_enabled(true), Node2D(name)
{
    this->metatable = "CollisionShape2DMetaTable";
}

void CollisionShape2D::Update()
{
    Node2D::Update();
    if (shape) {
        shape->setOrigin(getGlobalPosition().x, getGlobalPosition().y);
    }
}

void CollisionShape2D::setShape(Shape2D *new_shape) { shape.reset(new_shape); }

Shape2D *CollisionShape2D::getShape() const { return shape.get(); }

void CollisionShape2D::toggleCollision() { collision_enabled = !collision_enabled; }

bool CollisionShape2D::isCollisionEnabled() const { return collision_enabled; }

bool CollisionShape2D::collidesWith(const CollisionShape2D &other)
{
    if (!collision_enabled || !other.collision_enabled) {
        return false;
    }
    if (!shape || !other.shape) {
        return false;
    }

    if (!this->getBoundingBox().intersects(other.getBoundingBox())) {
        return false;
    }

    return shape->intersects(*other.getShape());
}

Types::Rect2 CollisionShape2D::getBoundingBox() const { return shape->getBoundingBox(); }
