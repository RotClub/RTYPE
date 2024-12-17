/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Node2D
*/

#ifndef NODE2D_HPP_
    #define NODE2D_HPP_

    #include "Nodes/Node.hpp"
    #include <raylib-cpp.hpp>

class Node2D : public Node {
    public:
        explicit Node2D(const std::string &name = "node2d");
        ~Node2D();

        Types::Vector2 position;

        Types::Vector2 getGlobalPosition() const { return _globalPosition; }

        void addChild(Node &child) override;

        void Update() override;

    private:
        Types::Vector2 _globalPosition;

        void _updateGlobalPosition();
};

#endif /* !NODE2D_HPP_ */
