/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Sprite2D
*/

#ifndef SPRITE2D_HPP_
#define SPRITE2D_HPP_

#include "../Node2D.hpp"
#include "Engine.hpp"
#include "Types.hpp"

class Sprite2D : public Node2D
{
    public:
        Sprite2D(const std::string &name = "sprite2d", const std::string &texture = "");
        ~Sprite2D();

        Sprite2D &SetTexture(const std::string &texture);
        Sprite2D &SetSource(const Types::Rect2 &source);
        Sprite2D &SetSize(const Types::Vector2 &size);

        void Draw() override;

    private:
        std::string _texture;
        Types::Rect2 _source;
        Types::Vector2 _size;
};

#endif /* !SPRITE2D_HPP_ */
