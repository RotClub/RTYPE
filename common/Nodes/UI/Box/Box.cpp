//
// Created by Yanis Moyon on 15/01/2025.
//

#include "Box.hpp"
#include "Engine.hpp"

Box::Box(const std::string &name, const Types::Vector2 &position, const Types::Vector2 &size) :
    UI(name, position), _size(size)
{
}

void Box::Draw()
{
    raylib::Color color = raylib::Color(red, green, blue, alpha);
    DrawRectangle(getGlobalPosition().x, getGlobalPosition().y, _size.x, _size.y, color);
}
