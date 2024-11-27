/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Sprite2D
*/

#include "Sprite2D.hpp"

Sprite2D::Sprite2D(const std::string &name, const std::string &texture)
    : texture(texture), Node2D(name)
{
}

Sprite2D::~Sprite2D()
{
}

Sprite2D &Sprite2D::SetTexture(const std::string &texture)
{
    this->texture = texture;
    return *this;
}
