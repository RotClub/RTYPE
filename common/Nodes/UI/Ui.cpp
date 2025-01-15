//
// Created by Yanis Moyon on 14/01/2025.
//

#include "Ui.hpp"
#include <Engine.hpp>

UI::UI(const std::string &name, const Types::Vector2 &position) :
    Node(name), position(position), red(255), green(255), blue(255), alpha(255)
{
}

void UI::addChild(Node &child)
{
    Node::addChild(child);
    _updateGlobalPosition();
}

void UI::Update() { _updateGlobalPosition(); }

void UI::_updateGlobalPosition()
{
    Node *parentNode = getParent();
    if (parentNode == nullptr) {
        _globalPosition = position;
        return;
    }
    if (UI *ui = dynamic_cast<UI *>(parentNode)) {
        _globalPosition = ui->getGlobalPosition() + position;
        return;
    }
    _globalPosition = position;
}
