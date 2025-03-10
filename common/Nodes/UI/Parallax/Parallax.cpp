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
#include "spdlog/spdlog.h"

Parallax::Parallax(const std::string &texture, const std::string &name, int zIndex, Node2D *referenceNode)
    : UI(name, {0, 0}), _referenceNode(referenceNode), _zIndex(zIndex)
{
    _zIndex = zIndex;
    setTexture(texture);
}

Parallax::~Parallax() {}

Parallax &Parallax::setTexture(const std::string &texture)
{
    this->_texture = (Engine::GetInstance().getGamePath() / texture).string();
    Engine::GetInstance().getResourceManager().loadResource(_texture);
    return *this;
}

void Parallax::setReferenceNode(Node2D *node) { _referenceNode = node; }

void Parallax::addParallaxPosition(const Types::Vector2 &pos) { _parallaxPos += pos; }

void Parallax::Draw()
{
    raylib::Texture &tex = Engine::GetInstance().getResourceManager().getTexture(_texture);
    float width = GetRenderWidth();
    float height = GetRenderHeight();
    if (_referenceNode == nullptr) {
        _drawPos.x += _parallaxPos.x;
        _drawPos.y += _parallaxPos.y;
        raylib::Rectangle src = {_drawPos.x, _drawPos.y, width, height};
        tex.Draw(src, {0, 0, width, height}, {0, 0}, 0);
        return;
    }
    Types::Vector2 pos = _referenceNode->getGlobalPosition();
    _drawPos.x += ((std::fmod(pos.x, width) + _parallaxPos.x) * _zIndex);
    _drawPos.y += ((std::fmod(pos.y, height) + _parallaxPos.y) * _zIndex);
    raylib::Rectangle src = {_drawPos.x, _drawPos.y, width, height};
    raylib::Rectangle dest = {0, 0, width, height};
    tex.Draw(src, dest, {0, 0}, 0);
    _drawPos = {0, 0};
}
