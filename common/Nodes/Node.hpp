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

class Node {
    public:
        Node(const std::string &name);
        ~Node();

        const Node &GetChild(const std::string &name) const;
        const std::vector<Node> &GetChildren() const;

        std::vector<Node> children;
        std::string name;
};

#endif /* !NODE_HPP_ */
