//
// Created by Yanis Moyon on 13/01/2025.
//

#include "Body2D.hpp"

void Body2D::Update() {
	CollisionNode2D::Update();
	for (auto &child: children)
	{
		if (Node2D *node = dynamic_cast<Node2D*>(child))
		{
			node->position += node->position * velocity;
		}
	}
}
