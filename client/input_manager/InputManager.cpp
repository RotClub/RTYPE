//
// Created by Théo Bary on 09/01/2025.
//

#include "InputManager.hpp"

#include <iostream>

InputManager::InputManager()
{
}

InputManager &InputManager::GetInstance()
{
    static InputManager instance;
    return instance;
}

void InputManager::update()
{
    std::vector<int> currentPressedKeys = {};
    while (int key = GetKeyPressed()) {
        currentPressedKeys.push_back(key);
    }
    _justPressedKeys.clear();
    for (int key : currentPressedKeys) {
        if (std::ranges::find(_pressedKeys, key) == _pressedKeys.end()) {
            _justPressedKeys.push_back(key);
        }
    }
    _pressedKeys = currentPressedKeys;
    std::cout << "Pressed keys: " << _pressedKeys.size() << std::endl;
}

bool InputManager::isKeyPressed(int key)
{
    return std::ranges::find(_pressedKeys, key) != _pressedKeys.end();
}

bool InputManager::isKeyJustPressed(int key)
{
    return std::ranges::find(_justPressedKeys, key) != _justPressedKeys.end();
}
