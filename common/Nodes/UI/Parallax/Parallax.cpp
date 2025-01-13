/*
** EPITECH PROJECT, 2025
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** Parallax
*/

#include "Parallax.hpp"
#include "Engine.hpp"
#include "Rectangle.hpp"
#include "Types.hpp"
#include "Vector2.hpp"

Parallax::Parallax(const std::string &texture, const std::string &name, int zIndex)
    : Node(name)
{
    _zIndex = zIndex;
    setTexture(texture);
}

Parallax::~Parallax()
{
}

Parallax &Parallax::setTexture(const std::string &texture)
{
    this->_texture = Engine::GetInstance().getGamePath() / texture;
    #if defined(CLIENT)
        Engine::GetInstance().getResourceManager().loadResource(_texture);
    #endif
    return *this;
}

void Parallax::Update()
{
}

void Parallax::Draw()
{
    Types::Vector2 pos = _referenceNode->getGlobalPosition();
    raylib::Texture &tex = Engine::GetInstance().getResourceManager().getTexture(_texture);
    float width = tex.width;
    float height = tex.height;
    raylib::Rectangle src = {std::fmod(pos.x, width), std::fmod(pos.y, height), width, height};
    raylib::Rectangle dest = {pos.x * _zIndex, pos.y * _zIndex, width, height};
    tex.Draw(src, dest, {0, 0}, 0);
}
