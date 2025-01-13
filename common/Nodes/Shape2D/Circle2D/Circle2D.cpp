/*
** EPITECH PROJECT, 2025
** RTYPE
** File description:
** Circle2D
*/

#include "Circle2D.hpp"
#include "Nodes/Shape2D/Rectangle2D/Rectangle2D.hpp"
#include "Utils.hpp"

Circle2D::Circle2D(const Types::Vector2 &origin, float radius)
    : _origin(origin), _radius(radius) {}


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


void Circle2D::setOrigin(float x, float y)
{
	_origin = Types::Vector2(x, y);
}

bool Circle2D::_intersectsCircle(const Shape2D &other) const
{
	const auto &circle = dynamic_cast<const Circle2D&>(other);
    float dx = _origin.x - other.getOrigin().x;
    float dy = _origin.y - other.getOrigin().y;
    
    float dist = dx * dx + dy * dy;
    float sumr = _radius + circle.getRadius();

    return dist <= sumr * sumr;
}

bool Circle2D::_intersectsRect(const Shape2D &rect) const
{
    return Geometric::intersectCircleWithRect(*this, dynamic_cast<const Rectangle2D&>(rect));
}

Types::Rect2 Circle2D::getBoundingBox() const
{
    Types::Vector2 bborigin = Types::Vector2(_origin.x - _radius, _origin.y + _radius);
    return Types::Rect2(bborigin, Types::Vector2(_radius * 2, _radius * 2));
}
