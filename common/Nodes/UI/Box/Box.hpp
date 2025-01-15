//
// Created by Yanis Moyon on 15/01/2025.
//

#ifndef BOX_HPP
#define BOX_HPP

#include <Nodes/UI/Ui.hpp>

class Box : public UI
{
    public:
        Box(const std::string &name, const Types::Vector2 &position, const Types::Vector2 &size);
        void Draw() override;

    private:
        Types::Vector2 _size;
};


#endif // BOX_HPP
