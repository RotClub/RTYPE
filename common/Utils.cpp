//
// Created by Yanis Moyon on 13/01/2025.
//

#include "Utils.hpp"
#include <iostream>

bool Geometric::intersectCircleWithRect(const Circle2D &circle, const Rectangle2D &rect) {
	// find closest point of the rectangle to the circle
	Types::Vector2 rect_origin = rect.getOrigin();
	Types::Vector2 rect_size = rect.getDimensions().size;
	Types::Vector2 circle_origin = circle.getOrigin();
	float closestX = std::max(rect_origin.x, std::min(circle_origin.x, rect_origin.x + rect_size.x));
	float closestY = std::max(rect_origin.y, std::min(circle_origin.y, rect_origin.y + rect_size.y));

	// Calculate the distance between the circle's center and this closest point
	float dx = circle_origin.x - closestX;
	float dy = circle_origin.y - closestY;
	float distanceSquared = dx * dx + dy * dy;

	// If the distance is less than the circle's radius = intersection
	return distanceSquared <= circle.getRadius() * circle.getRadius();
}
