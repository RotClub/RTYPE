/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Node
*/

#ifndef NODE_HPP_
    #define NODE_HPP_

    #include <vector>
    #include <string>
    #include <stdexcept>
    #include "../Types.hpp"

class Node {
    public:
        Node(const std::string &name);
        ~Node();

        const Node &GetChild(const std::string &name) const;
        const std::vector<Node *> &GetChildren() const;

        void AddChild(Node &child) { children.push_back(&child); }

        virtual void Draw();

        std::vector<Node *> children;
        std::string name;
};

#endif /* !NODE_HPP_ */
