/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** Game
*/

#include "Game.hpp"

#include <config/Config.hpp>

#include "../Client.hpp"
#include "Networking/Packet.hpp"
#include "Text.hpp"

Game::Game() {}

Game::~Game()
{
    _audioDevice.Close();
}

void Game::run()
{
    SetAudioStreamBufferSizeDefault(4096);
    _window.Init(800, 600, "Connecting to server...");
    _window.SetPosition(GetScreenWidth() / 2, GetScreenHeight() / 2);
    _window.SetExitKey(KeyboardKey::KEY_NULL);
    if (_window.IsReady() == false)
        throw std::runtime_error("Window is not ready");
    if (_audioDevice.IsReady() == false)
        throw std::runtime_error("Audio device is not ready");
    Engine::GetInstance().clientStarted = true;
    Client &client = Client::GetInstance();
    client.getClientConnection().establishConnection();
    client.setupLua();
    while (!client.isConnectionEstablished()) {
        _renderPreGameText("Connecting to server...");
        if (_window.ShouldClose())
            return;
        client.processIncomingPackets();
    }
    client.loadLuaGame();
    _loadResources();

    const bool accessibilityShaderExists =
        exists(Engine::GetInstance().getGamePath() / "assets/shaders/accessibility.fs");
    if (accessibilityShaderExists)
        _accessibilityLoop();
    else
        _loop();
}

const Types::Vector2 &Game::getWindowSize() const
{
    _windowSize = Types::Vector2(_window.GetHeight(), _window.GetWidth());
    return _windowSize;
}

void Game::_loop()
{
    while (!_shouldClose) {
        int dt = Engine::GetInstance().deltaTime();
        _update(dt);
        _window.BeginDrawing();
        _draw(dt);
        _window.ClearBackground(raylib::Color::Black());
        if (Config::GetInstance().getFpsCounter())
            DrawFPS(2, 2);
        _window.EndDrawing();
        if (_window.ShouldClose())
            _shouldClose = true;
    }
}

void Game::_accessibilityLoop()
{
    raylib::RenderTexture2D target = raylib::RenderTexture2D(_window.GetWidth(), _window.GetHeight());
    Shader accessibilityShader =
        LoadShader(nullptr, (Engine::GetInstance().getGamePath() / "assets/shaders/accessibility.fs").string().c_str());
    int shaderModeLoc = GetShaderLocation(accessibilityShader, "mode");
    int shaderIntensityLoc = GetShaderLocation(accessibilityShader, "intensity");
    const float shaderIntensity = 1.0f;
    SetShaderValue(accessibilityShader, shaderIntensityLoc, &shaderIntensity, SHADER_UNIFORM_FLOAT);
    while (!_shouldClose) {
        int dt = Engine::GetInstance().deltaTime();
        SetShaderValue(accessibilityShader, shaderModeLoc, Config::GetInstance().getColorBlindnessMode(),
                       SHADER_UNIFORM_INT);
        _update(dt);
        target.BeginMode();
        _draw(dt);
        if (Config::GetInstance().getFpsCounter())
            DrawFPS(2, 2);
        target.EndMode();
        _window.BeginDrawing();
        _window.ClearBackground(raylib::Color::Black());
        BeginShaderMode(accessibilityShader);
#ifdef WIN32
        DrawTextureRec(target.texture, Rectangle{0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height)},
               Vector2{0, 0}, WHITE);
#else
        DrawTextureRec(target.texture, (Rectangle){0, 0, static_cast<float>(target.texture.width), static_cast<float>(-target.texture.height)},
                       (Vector2){0, 0}, WHITE);
#endif
        EndShaderMode();
        _window.EndDrawing();
        if (_window.ShouldClose())
            _shouldClose = true;
    }
    UnloadShader(accessibilityShader);
    UnloadRenderTexture(target);
}

void Game::_update(int dt)
{
    Client &client = Client::GetInstance();
    Engine::GetInstance().callHook("Tick", "int", dt, nullptr);
    client.broadcastLuaPackets();
    client.processIncomingPackets();
    Node *rootNode = Engine::GetInstance().root;
    _updateNodes(*rootNode);
}

void Game::_draw(int dt)
{
    _window.ClearBackground(raylib::Color::Black());
    _drawNodes(*Engine::GetInstance().root);
}

void Game::_updateNodes(Node &node)
{
    if (&node == nullptr)
        return;
    node.Update();
    if (node.GetChildren().size() == 0)
        return;
    for (auto child : node.GetChildren()) {
        _updateNodes(*child);
    }
}

void Game::_drawNodes(Node &node)
{
    if (&node == nullptr)
        return;
    node.Draw();
    if (node.GetChildren().size() == 0)
        return;
    for (auto child : node.GetChildren()) {
        _drawNodes(*child);
    }
}

void Game::_loadResources()
{
    ResourceManager &resourceManager = Engine::GetInstance().getResourceManager();
    resourceManager.loadAllPendingResources();
}

void Game::_renderPreGameText(std::string text)
{
    _window.ClearBackground(raylib::Color::Black());
    raylib::Text textObj = raylib::Text(text, 20);
    int textWidth = textObj.MeasureEx().GetX();
    int textHeight = textObj.MeasureEx().GetY();
    textObj.Draw(_window.GetWidth() / 2 - textWidth / 2, _window.GetHeight() / 2 - textHeight / 2);
}
