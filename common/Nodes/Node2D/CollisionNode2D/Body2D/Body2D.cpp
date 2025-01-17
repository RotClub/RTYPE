//
// Created by Yanis Moyon on 13/01/2025.
//

#include "Body2D.hpp"

#include <Engine.hpp>

Body2D::Body2D(const std::string &name) : CollisionNode2D(name) { this->metatable = "Body2DMetaTable"; }

void Body2D::Update()
{
    float dt = Engine::GetInstance().getDeltaLast();
    position += velocity * dt;
    CollisionNode2D::Update();
}
