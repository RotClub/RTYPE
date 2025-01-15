//
// Created by Yanis Moyon on 14/01/2025.
//

#ifndef UI_HPP
#define UI_HPP

#include "Nodes/Node.hpp"

class UI : public Node
{
    public:
        explicit UI(const std::string &name = "ui", const Types::Vector2 &position = {0, 0});
        ~UI() override = default;

        Types::Vector2 position;

        Types::Vector2 getGlobalPosition() const { return _globalPosition; }

        void addChild(Node &child) override;

        void Update() override;

        void setRGB(const Types::Vector3 &color) { _color = color; }
        void setAlpha(float alpha) { this->alpha = alpha; }

        [[nodiscard]] const Types::Vector3 &getRGB() const { return _color; }
        [[nodiscard]] float getAlpha() const { return alpha; }

    private:
        Types::Vector2 _globalPosition;

        Types::Vector3 _color;

        float alpha;

        void _updateGlobalPosition();
};


#endif // UI_HPP
