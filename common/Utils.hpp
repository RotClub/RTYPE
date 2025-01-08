/*
** EPITECH PROJECT, 2025
** RTYPE
** File description:
** Utils
*/

#include "Nodes/Shape2D/Shape2D.hpp"
#include <iostream>

namespace Geometric {

    bool intersectCircleWithRect(const Circle2D &circle, const Rectangle2D &rect)
    {
        // find closest point of the rectangle to the circle
        float closestX = std::max(rect.origin.x, std::min(circle.getOrigin().x, rect.origin.x + rect.size.x));
        float closestY = std::max(rect.origin.y, std::min(circle.getOrigin().y, rect.origin.y + rect.size.y));

        // Calculate the distance between the circle's center and this closest point
        float dx = circle.getOrigin().x - closestX;
        float dy = circle.getOrigin().y - closestY;
        float distanceSquared = dx * dx + dy * dy;

        // If the distance is less than the circle's radius = intersection
        return distanceSquared <= (circle.getRadius() * circle.getRadius());
    }
}
