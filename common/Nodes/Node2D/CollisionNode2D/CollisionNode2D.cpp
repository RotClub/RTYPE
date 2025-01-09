//
// Created by Yanis Moyon on 07/01/2025.
//

#include "CollisionNode2D.hpp"
#include <algorithm>

CollisionNode2D::CollisionNode2D(const std::string &name)
	: shapes(), collision_enabled(true), Node2D(name) {
	this->metatable = "CollisionNode2DMetaTable";
}

void CollisionNode2D::addCollisionShape(CollisionShape2D* new_shape) {
	shapes.push_back(std::unique_ptr<CollisionShape2D>(new_shape));
}

void CollisionNode2D::removeCollisionShape(const std::string &name) {
	shapes.erase(std::remove_if(shapes.begin(), shapes.end(), [name](const std::unique_ptr<CollisionShape2D> &shape) {
		return shape->name == name;
	}), shapes.end());
}

CollisionShape2D* CollisionNode2D::getCollisionShape(const std::string &name) const {
    for (const auto& shape : shapes) {
        if (shape->name == name) {
            return shape.get();
        }
    }
    return nullptr;
}

std::vector<CollisionShape2D*> CollisionNode2D::getCollisionShapes() const {
	std::vector<CollisionShape2D*> shapes_ptr;
	for (const auto &shape : shapes) {
		shapes_ptr.push_back(shape.get());
	}
	return shapes_ptr;
}

Types::Rect2 CollisionNode2D::getBoundingBox() const {
	Types::Rect2 bounding_box;
	for (const auto &shape : shapes) {
		bounding_box.expands(shape->getBoundingBox());
	}
	return bounding_box;
}

void CollisionNode2D::toggleCollision() {
	collision_enabled = !collision_enabled;
	for (auto &shape : shapes) {
		shape->toggleCollision();
	}
}

bool CollisionNode2D::isCollisionEnabled() const {
	return collision_enabled;
}

bool CollisionNode2D::collidesWith(const CollisionNode2D& other) {
	if (!collision_enabled || !other.collision_enabled) {
		return false;
	}
	for (const auto &shape : shapes) {
		for (const auto &other_shape : other.shapes) {
			if (shape->collidesWith(*other_shape)) {
				return true;
			}
		}
	}
	return false;
}
