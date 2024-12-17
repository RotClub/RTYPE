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

class Sprite2D : public Node2D {
    public:
        Sprite2D(const std::string &name, const std::string &texture);
        ~Sprite2D();

        Sprite2D &SetTexture(const std::string &texture);

        void Draw() override;

    private:
        std::string _texture;
};

#endif /* !SPRITE2D_HPP_ */
