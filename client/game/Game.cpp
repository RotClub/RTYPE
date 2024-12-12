/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** Game
*/

#include "Game.hpp"
#include "../Client.hpp"

Game::Game(Client &client)
    : _client(client)
{
}

void Game::run()
{
    _window = raylib::Window(1920, 1080, "RType");
    _window.SetTargetFPS(60);
    _window.SetSize(1920, 1080);
    _window.SetPosition(0, 0);
    _window.SetExitKey(KeyboardKey::KEY_NULL);
    _loadResources();
    while (!_window.ShouldClose()) {
        int dt = Engine::GetInstance().deltaTime();
        _window.BeginDrawing();
        _update(dt);
        _window.EndDrawing();
    }
}

void Game::_update(int dt)
{
    _window.ClearBackground(raylib::Color::White());
    raylib::Text("Hello, World!").Draw(raylib::Vector2(10, 10));
    raylib::Text("Delta Time: " + std::to_string(dt)).Draw(raylib::Vector2(10, 30));
}

void Game::_loadResources()
{
    ResourceManager &resourceManager = _client.getResourceManager();
}
