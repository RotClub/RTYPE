/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Node2D
*/

#include "Node2D.hpp"

Node2D::Node2D(const std::string &name)
    : position(Types::Vector2::Zero()), Node(name)
{
}

Node2D::~Node2D()
{
}
