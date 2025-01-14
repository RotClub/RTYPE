//
// Created by Yanis Moyon on 14/01/2025.
//

#ifndef STATICBODY2D_HPP
#define STATICBODY2D_HPP

#include <Nodes/Node2D/CollisionNode2D/Body2D/Body2D.hpp>

class StaticBody2D : public Body2D
{
    public:
        StaticBody2D(const std::string &name, const Types::Vector2 &position);
};


#endif // STATICBODY2D_HPP
