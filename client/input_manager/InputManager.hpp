//
// Created by Théo Bary on 09/01/2025.
//

#ifndef INPUTMANAGER_HPP
    #define INPUTMANAGER_HPP

    #include "raylib-cpp.hpp"
    #include <map>

class InputManager {
    public:
        InputManager(InputManager const &) = delete;
        void operator=(InputManager const &) = delete;

        static InputManager &GetInstance();

        void update();
        bool isKeyPressed(int key);
        bool isKeyJustPressed(int key);

    protected:
        InputManager();

    private:
        std::vector<int> _pressedKeys;
        std::vector<int> _justPressedKeys;
};

#endif //INPUTMANAGER_HPP
