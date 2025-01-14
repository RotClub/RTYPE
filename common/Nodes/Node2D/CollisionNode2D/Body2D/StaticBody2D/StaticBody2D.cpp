//
// Created by Yanis Moyon on 14/01/2025.
//

#include "StaticBody2D.hpp"

StaticBody2D::StaticBody2D(const std::string& name, const Types::Vector2& position) : Body2D(name)
{
    this->metatable = "StaticBody2DMetaTable";
    this->position = position;
    this->velocity = Types::Vector2(0, 0);
}
