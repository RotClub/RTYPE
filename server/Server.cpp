/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Server
*/

#include "Server.hpp"

Server::Server(int port) : _port(port), _isRunning(false), _serverConnectionTcp(port), _serverConnectionUdp(port, true)
{
}

bool Server::start()
{
    try {
        _serverConnectionTcp.start();
        _serverConnectionUdp.start();
        _isRunning = true;
        return true;
    } catch (const std::exception &e) {
        std::cerr << "[Server] Error starting server: " << e.what() << std::endl;
        return false;
    }
}

void Server::stop()
{
    _serverConnectionTcp.stop();
    _serverConnectionUdp.stop();
    _isRunning = false;
}

int Server::getPort() const
{
    return _port;
}
