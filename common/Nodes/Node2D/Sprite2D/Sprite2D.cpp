/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Sprite2D
*/

#include "Sprite2D.hpp"
#include "Rectangle.hpp"

Sprite2D::Sprite2D(const std::string &name, const std::string &texture) : Node2D(name)
{
    SetTexture(texture);
    this->metatable = "Sprite2DMetaTable";
}

Sprite2D::~Sprite2D() {}

Sprite2D &Sprite2D::SetTexture(const std::string &texture)
{
    this->_texture = Engine::GetInstance().getGamePath() / texture;
    if (Engine::GetInstance().getState() == Types::VMState::CLIENT)
        Engine::GetInstance().getResourceManager().loadResource(_texture);
    return *this;
}

Sprite2D &Sprite2D::SetSize(const Types::Vector2 &size)
{
    _size = size;
    return *this;
}

Sprite2D &Sprite2D::SetSource(const Types::Rect2 &source)
{
    _source = source;
    return *this;
}

void Sprite2D::Draw()
{
    if (Engine::GetInstance().getState() == Types::VMState::SERVER)
        return;
    raylib::Texture &tex = Engine::GetInstance().getResourceManager().getTexture(_texture);
    float width = tex.width;
    float height = tex.height;
    raylib::Rectangle src;
    if (_source.size.x == 0 && _source.size.y == 0) {
        raylib::Rectangle tmp = {0, 0, width, height};
        src = tmp;
    }
    else {
        raylib::Rectangle tmp = {_source.origin.x, _source.origin.y, _source.size.x, _source.size.y};
        src = tmp;
    }
    Types::Vector2 finalSize = _size;
    if (finalSize.x == 0 && finalSize.y == 0) {
        finalSize = {_source.size.x, _source.size.y};
    }
    tex.Draw(src, {getGlobalPosition().x, getGlobalPosition().y, finalSize.x, finalSize.y}, {0, 0}, 0);
}
