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

        Node* getParent() { return _parent; }

        virtual void addChild(Node &child);

        virtual void Update();
        virtual void Draw();

        std::vector<Node *> children;
        std::string name;
        std::string metatable;

    private:
        Node *_parent = nullptr;
};

#endif /* !NODE_HPP_ */
