/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Client
*/

#include "Client.hpp"
#include "game/Game.hpp"

Client *Client::_instance = nullptr;

Client &Client::InitiateInstance(std::string ip, int port)
{
    if (_instance != nullptr)
        return Client::GetInstance();
    _instance = new Client(ip, port);
    return *_instance;
}

Client &Client::GetInstance()
{
    if (_instance == nullptr)
        throw std::runtime_error("Instance not initiated");
    return *_instance;
}

Client::Client(std::string ip, int port)
    : _ip(ip), _port(port),
    _clientConnectionTcp(ip, port, false), _clientConnectionUdp(ip, port, true)
{
}

void Client::startGame()
{
    _game.run();
}

std::string Client::getIp() const
{
    return _ip;
}

int Client::getPort() const {
    return _port;
}

ClientConnection &Client::getClientConnectionTcp()
{
    return _clientConnectionTcp;
}

ClientConnection &Client::getClientConnectionUdp()
{
    return _clientConnectionUdp;
}
