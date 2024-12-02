/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Client
*/

#include "Client.hpp"

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
{
    _ip = ip;
    _port = port;
    _serverConnection = new ClientConnection(ip, port);
}

std::string Client::getIp() const {
    return _ip;
}

int Client::getPort() const {
    return _port;
}

