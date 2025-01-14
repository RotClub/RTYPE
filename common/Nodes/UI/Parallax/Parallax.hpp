/*
** EPITECH PROJECT, 2025
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** Parallax
*/

#ifndef PARALLAX_HPP_
    #define PARALLAX_HPP_

    #include "Engine.hpp"
    #include "Nodes/Node2D/Node2D.hpp"
    #include "Nodes/UI/Ui.hpp"
    #include <string>

class Parallax : public UI {
    public:
        Parallax(const std::string &texture, const std::string &name = "parallax", int zIndex = 0, Node2D *referenceNode = nullptr);
        ~Parallax();

        void Draw() override;

        Parallax &setTexture(const std::string &texture);
        void setReferenceNode(Node2D *node);
        void addParallaxPosition(const Types::Vector2 &pos);

    private:
        std::string _texture;
        Node2D *_referenceNode;
        int _zIndex;
        Types::Vector2 _drawPos;
        Types::Vector2 _parallaxPos;
};

#endif /* !PARALLAX_HPP_ */
