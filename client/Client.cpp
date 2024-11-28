/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Client
*/

#include "Client.hpp"

Client *Client::_instance = nullptr;

Client *Client::InitiateInstance(std::string ip, int port)
{
    if (_instance != nullptr)
        return nullptr;
    _instance = new Client(ip, port);
    return _instance;
}

Client *Client::GetInstance()
{
    if (_instance == nullptr)
        return nullptr;
    return _instance;
}

Client::Client(std::string ip, int port)
{
    _ip = ip;
    _port = port;
}

