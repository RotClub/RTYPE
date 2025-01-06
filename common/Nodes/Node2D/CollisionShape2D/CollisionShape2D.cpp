//
// Created by Yanis Moyon on 18/12/2024.
//

#include "CollisionShape2D.hpp"

CollisionShape2D::CollisionShape2D()
    : shape(nullptr), collision_enabled(true) {
    this->metatable = "CollisionShape2DMetaTable";
}

void CollisionShape2D::setShape(Shape2D* new_shape) {
    shape = new_shape;
}

Shape2D* CollisionShape2D::getShape() const {
    return shape;
}

void CollisionShape2D::toggleCollision() {
    collision_enabled = !collision_enabled;
}

bool CollisionShape2D::isCollisionEnabled() const {
    return collision_enabled;
}

bool CollisionShape2D::collidesWith(const CollisionShape2D& other) {
    if (!collision_enabled || !other.collision_enabled || !shape || !other.shape) {
        return false;
    }

	if (!shape->getBoundingBox().intersects(other.shape->getBoundingBox())) {
		return false;
	}

    return shape->intersects(*other.getShape());
}
