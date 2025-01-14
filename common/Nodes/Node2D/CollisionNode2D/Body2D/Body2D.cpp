//
// Created by Yanis Moyon on 13/01/2025.
//

#include "Body2D.hpp"

#include <Engine.hpp>

Body2D::Body2D(const std::string& name) : CollisionNode2D(name) {
	this->metatable = "Body2DMetaTable";
}

void Body2D::Update() {
	float dt = Engine::GetInstance().getDeltaLast();
	CollisionNode2D::Update();
	position += velocity * dt;
	for (auto &child: children)
	{
		if (Node2D *node = dynamic_cast<Node2D*>(child))
		{
			node->position += velocity * dt;
		}
	}
}
