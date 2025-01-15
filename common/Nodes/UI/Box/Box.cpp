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
    Types::Vector3 this_color = getRGB();
    raylib::Color color = raylib::Color(this_color.x, this_color.y, this_color.z, getAlpha());
    DrawRectangle(getGlobalPosition().x, getGlobalPosition().y, _size.x, _size.y, color);
}
