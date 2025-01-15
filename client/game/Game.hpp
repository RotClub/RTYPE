/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** Game
*/

#ifndef GAME_HPP_
#define GAME_HPP_

#include "../../common/Engine.hpp"
#include "Networking/Packet.hpp"

#include <raylib-cpp.hpp>

class Game
{
    public:
        Game();
        ~Game();

        void run();

    private:
        void _update(int dt);
        void _loop();
        void _accessibilityLoop();
        void _draw(int dt);
        void _updateNodes(Node &node);
        void _drawNodes(Node &node);
        void _loadResources();

        raylib::Window _window;
        bool _shouldClose = false;
};

#endif /* !GAME_HPP_ */
