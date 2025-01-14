//
// Created by Yanis Moyon on 14/01/2025.
//

#ifndef UI_HPP
#define UI_HPP

#include "Nodes/Node.hpp"

class UI : public Node
{
public:
    explicit UI(const std::string &name = "ui", const Types::Vector2 &position={0, 0});
    ~UI() override = default;
private:
    Types::Vector2 _position;
};


#endif // UI_HPP
