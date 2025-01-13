//
// Created by Yanis Moyon on 13/01/2025.
//

#include "RigidBody2D.hpp"

RigidBody2D::RigidBody2D(const std::string& name, const Types::Vector2& position, const Types::Vector2& velocity)
	: Body2D(name) {
	this->metatable = "RigidBody2DMetaTable";
	this->velocity = velocity;
	this->position = position;
}
