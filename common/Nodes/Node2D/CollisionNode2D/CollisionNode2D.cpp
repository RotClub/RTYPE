//
// Created by Yanis Moyon on 07/01/2025.
//

#include "CollisionNode2D.hpp"
#include <algorithm>

CollisionNode2D::CollisionNode2D(const std::string &name)
	: collision_enabled(true), Node2D(name) {
	this->metatable = "CollisionNode2DMetaTable";
}

void CollisionNode2D::Update() {
	Node2D::Update();
}

Types::Rect2 CollisionNode2D::getBoundingBox() const {
	Types::Rect2 bounding_box;
	for (const auto &shape : children) {
		if (dynamic_cast<CollisionNode2D*>(shape))
		{
			bounding_box.expands(dynamic_cast<CollisionNode2D*>(shape)->getBoundingBox());
		}
	}
	return bounding_box;
}

void CollisionNode2D::toggleCollision() {
	collision_enabled = !collision_enabled;
	for (auto &shape : children) {
		if (dynamic_cast<CollisionNode2D*>(shape))
		{
			dynamic_cast<CollisionNode2D*>(shape)->toggleCollision();
		}
	}
}

bool CollisionNode2D::isCollisionEnabled() const {
	return collision_enabled;
}

bool CollisionNode2D::collidesWith(const CollisionNode2D& other) {
	if (!collision_enabled || !other.collision_enabled) {
		return false;
	}
	for (const auto &shape : children) {
		for (const auto &other_shape : other.children) {
			if (dynamic_cast<CollisionNode2D*>(shape) && dynamic_cast<CollisionNode2D*>(other_shape))
			{
				if (dynamic_cast<CollisionNode2D*>(shape)->collidesWith(*dynamic_cast<CollisionNode2D*>(other_shape))) {
					return true;
				}
			}
		}
	}
	return false;
}
