/*
** EPITECH PROJECT, 2025
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** Parallax
*/

#ifndef PARALLAX_HPP_
    #define PARALLAX_HPP_

    #include "Nodes/Node.hpp"
    #include "Nodes/Node2D/Node2D.hpp"
#include "Types.hpp"
    #include <memory>
    #include <string>

class Parallax : public Node {
    public:
        Parallax(const std::string &texture, const std::string &name = "parallax", int zIndex = 0);
        ~Parallax();

        void Update() override;
        void Draw() override;

        Parallax &setTexture(const std::string &texture);
        void setReferenceNode(Node2D *node);

    private:
        std::string _texture;
        Node2D *_referenceNode;
        int _zIndex;
        Types::Vector2 _drawPos;
};

#endif /* !PARALLAX_HPP_ */
