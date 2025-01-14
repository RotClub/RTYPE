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

    Types::Vector2 position;

    Types::Vector2 getGlobalPosition() const { return _globalPosition; }

    void addChild(Node &child) override;

    void Update() override;

    virtual void setRGB(float red, float green, float blue) { this->red = red; this->green = green; this->blue = blue; }
    virtual void setAlpha(float alpha) { this->alpha = alpha; }

    float red, green, blue, alpha;

private:
    Types::Vector2 _globalPosition;

    void _updateGlobalPosition();
};


#endif // UI_HPP
