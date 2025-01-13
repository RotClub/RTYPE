/*
** EPITECH PROJECT, 2025
** RTYPE
** File description:
** Shape2D
*/

#include "Shape2D.hpp"

bool Shape2D::intersects(const Shape2D &other) const
{
    if (shape == ShapeType::RECTANGLE)
	{
		return _intersectsRect(other);
	}
	if (shape == ShapeType::CIRCLE)
	{
		return _intersectsCircle(other);
	}
	return false;
}
