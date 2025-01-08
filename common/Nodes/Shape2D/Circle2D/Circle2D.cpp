/*
** EPITECH PROJECT, 2025
** RTYPE
** File description:
** Circle2D
*/

#include "Circle2D.hpp"

Circle2D::Circle2D()
{
}

Circle2D::~Circle2D()
{
}

void Circle2D::setRadius(float radius)
{
    _radius = radius;
}

float Circle2D::getRadius() const
{
    return _radius;
}

Types::Vector2 Circle2D::getOrigin() const
{
    return _origin;
}

void Circle2D::setOrigin(const Types::Vector2 &origin)
{
    _origin = origin;
}

