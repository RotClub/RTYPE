/*
** EPITECH PROJECT, 2025
** RTYPE
** File description:
** Utils
*/

#ifndef UTILS_HPP
#define UTILS_HPP

#include "Nodes/Shape2D/Circle2D/Circle2D.hpp"
#include "Nodes/Shape2D/Rectangle2D/Rectangle2D.hpp"

namespace Geometric {

    bool intersectCircleWithRect(const Circle2D &circle, const Rectangle2D &rect);
}

#endif //UTILS_HPP
