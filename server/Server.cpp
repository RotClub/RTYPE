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

int Server::getPort() const
{
    return _port;
}
