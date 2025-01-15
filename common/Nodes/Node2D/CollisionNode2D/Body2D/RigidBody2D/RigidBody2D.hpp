//
// Created by Yanis Moyon on 13/01/2025.
//

#ifndef RIGIDBODY2D_HPP
#define RIGIDBODY2D_HPP

#include <Nodes/Node2D/CollisionNode2D/Body2D/Body2D.hpp>

class RigidBody2D : public Body2D
{
    public:
        RigidBody2D(const std::string &name, const Types::Vector2 &position, const Types::Vector2 &velocity);
};

#endif // RIGIDBODY2D_HPP
