/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** Game
*/

#ifndef GAME_HPP_
#define GAME_HPP_

#include <raylib-cpp.hpp>

#include <Engine.hpp>
#include "Networking/Packet.hpp"

class Game
{
    public:
        Game();
        ~Game();

        void run();

        [[nodiscard]] const Types::Vector2 &getWindowSize() const;
        [[nodiscard]] raylib::Window &getWindow() { return _window; }

    private:
        void _update(int dt);
        void _loop();
        void _accessibilityLoop();
        void _draw(int dt);
        void _updateNodes(Node &node);
        void _drawNodes(Node &node);
        void _loadResources();
        void _renderPreGameText(std::string text);

        raylib::Window _window;
        mutable Types::Vector2 _windowSize;
        raylib::AudioDevice _audioDevice;
        bool _shouldClose = false;
};

#endif /* !GAME_HPP_ */
