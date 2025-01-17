//
// Created by Yanis Moyon on 13/01/2025.
//

#ifndef BODY2D_HPP
#define BODY2D_HPP

#include "Nodes/Node2D/CollisionNode2D/CollisionNode2D.hpp"

class Body2D : public CollisionNode2D
{
    public:
        explicit Body2D(const std::string &name);

        void Update() override;

        Types::Vector2 velocity;
};

#endif // BODY2D_HPP
