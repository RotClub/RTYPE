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

bool Server::start()
{
    try {
        _serverConnection.start();
        _isRunning = true;
        return true;
    } catch (const std::exception &e) {
        Engine::GetInstance().Log(Engine::LogLevel::ERROR, "Error starting server: " + std::string(e.what()));
        return false;
    }
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
