//
// Created by Yanis Moyon on 18/12/2024.
//

#include "CollisionShape2D.hpp"


CollisionShape2D::CollisionShape2D(const std::string &name)
    : shape(nullptr), collision_enabled(true), Node2D(name) {
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
    if (!collision_enabled || !other.collision_enabled) {
		std::cout << "Init condition" << std::endl;
        return false;
    }
	if (!shape || !other.shape)
	{
		std::cout << "First condition" << std::endl;
		return false;
	}

	if (!this->getBoundingBox().intersects(other.getBoundingBox())) {
		std::cout << "Second condition" << std::endl;
		return false;
	}

    return shape->intersects(*other.getShape());
}

Types::Rect2 CollisionShape2D::getBoundingBox() const {
	return shape->getBoundingBox();
}
