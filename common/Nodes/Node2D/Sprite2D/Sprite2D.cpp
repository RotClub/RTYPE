/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Sprite2D
*/

#include "Sprite2D.hpp"

Sprite2D::Sprite2D(const std::string& name, const std::string& texture) : Node2D(name)
{
    SetTexture(texture);
    this->metatable = "Sprite2DMetaTable";
}

Sprite2D::~Sprite2D() {}

Sprite2D& Sprite2D::SetTexture(const std::string& texture)
{
    this->_texture = Engine::GetInstance().getGamePath() / texture;
    if (Engine::GetInstance().getState() == Types::VMState::CLIENT)
        Engine::GetInstance().getResourceManager().loadResource(_texture);
    return *this;
}

void Sprite2D::Draw()
{
    if (Engine::GetInstance().getState() == Types::VMState::SERVER)
        return;
    raylib::Texture& tex = Engine::GetInstance().getResourceManager().getTexture(_texture);
    tex.Draw(getGlobalPosition().x, getGlobalPosition().y);
}
