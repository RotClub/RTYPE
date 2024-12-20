/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Server
*/

#include "Server.hpp"

#include <Engine.hpp>

Server::Server(int port)
    : _port(port), _isRunning(false), _serverConnection(port)
{
}

Server::~Server()
{
    stop();
}

void Server::start()
{
    try {
        Engine &engine = Engine::StartInstance(Types::VMState::SERVER, "rtype");
        engine.Log(Engine::LogLevel::INFO, "Server starting...");
        engine.displayGameInfo();
        engine.loadLibraries();
        if (engine.LoadLuaFile("index.luau"))
            engine.execute();
        engine.Log(Engine::LogLevel::INFO, "Server started!");
        engine.callHook("RType:InitServer", nullptr);
        _serverConnection.start();
        _isRunning = true;
        while (_isRunning) {
            loop();
        }
    } catch (const std::exception &e) {
        Engine::GetInstance().Log(Engine::LogLevel::ERROR, "Error starting server: " + std::string(e.what()));
        throw;
    }
}

void Server::loop()
{
    Engine &engine = Engine::GetInstance();
    engine.callHook("RType:Tick", "int", engine.deltaTime(), nullptr);
}

void Server::stop()
{
    _serverConnection.stop();
    _isRunning = false;
}

int Server::getPort() const
{
    return _port;
}
