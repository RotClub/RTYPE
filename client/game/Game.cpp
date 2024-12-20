/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** Game
*/

#include "Game.hpp"
#include "../Client.hpp"

Game::Game()
{
}

void Game::run()
{
    _window = raylib::Window();
    const std::string gameName = Engine::GetInstance().getGameInfo()->getName();
    _window.Init(800, 600, gameName);
    _window.SetPosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
    _window.SetExitKey(KeyboardKey::KEY_NULL);
    if (_window.IsReady() == false)
        throw std::runtime_error("Window is not ready");
    Engine::GetInstance().clientStarted = true;
    _loadResources();
    while (!_shouldClose) {
        int dt = Engine::GetInstance().deltaTime();
        _window.BeginDrawing();
        _update(dt);
        _window.EndDrawing();
        if (_window.ShouldClose())
            _shouldClose = true;
    }
}

void Game::_update(int dt)
{
    Node *rootNode = Engine::GetInstance().root;
    _window.ClearBackground(raylib::Color::Black());
    _updateNodes(*rootNode);
}

void Game::_updateNodes(Node &node)
{
    if (&node == nullptr)
        return;
    node.Update();
    node.Draw();
    if (node.GetChildren().size() == 0)
        return;
    for (auto child : node.GetChildren()) {
        _updateNodes(*child);
    }
}

void Game::_loadResources()
{
    ResourceManager &resourceManager = Engine::GetInstance().getResourceManager();
    resourceManager.loadAllPendingResources();
}
