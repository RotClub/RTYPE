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

Parallax::Parallax(const std::string &texture, const std::string &name, int zIndex, Node2D *referenceNode)
    : Node(name), _referenceNode(referenceNode), _zIndex(zIndex)
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
    Engine::GetInstance().getResourceManager().loadResource(_texture);
    return *this;
}

void Parallax::setReferenceNode(Node2D *node)
{
    _referenceNode = node;
}

void Parallax::Update()
{
}

void Parallax::Draw()
{
    raylib::Texture &tex = Engine::GetInstance().getResourceManager().getTexture(_texture);
    if (_referenceNode == nullptr) {
        tex.Draw(0, 0);
        return;
    }
    Types::Vector2 pos = _referenceNode->getGlobalPosition();
    float width = tex.width;
    float height = tex.height;
    raylib::Rectangle src = {std::fmod(pos.x, width) * _zIndex, std::fmod(pos.y, height) * _zIndex, width, height};
    raylib::Rectangle dest = {0, 0, width, height};
    tex.Draw(src, dest, {0, 0}, 0);
}
