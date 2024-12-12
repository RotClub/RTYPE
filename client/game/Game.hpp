/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** Game
*/

#ifndef GAME_HPP_
    #define GAME_HPP_

    #include "../../common/Engine.hpp"
    #include "../ResourceManager.hpp"

    #include <raylib-cpp.hpp>

class Client; // Forward declaration

class Game {
    public:
        Game(Client &client);
        ~Game();

        void run();

    private:
        void _update(int dt);
        void _loadResources();

        raylib::Window _window;
        Client &_client;
};

#endif /* !GAME_HPP_ */
