/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Node2D
*/

#ifndef NODE2D_HPP_
    #define NODE2D_HPP_

    #include "../Node.hpp"

class Node2D : public Node {
    public:
        Node2D(const std::string &name);
        ~Node2D();

        Types::Vector2 position;
};

#endif /* !NODE2D_HPP_ */
