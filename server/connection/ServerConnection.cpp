/*
** EPITECH PROJECT, 2024
** Rtype [WSL: Ubuntu]
** File description:
** ServerConnection
*/

#include "ServerConnection.hpp"

ServerConnection::ServerConnection(int port, bool udp)
    : GlobalConnection(), _port(port), _udp(udp), _running(false)
{
}

ServerConnection::~ServerConnection()
{
    stop();
}

void ServerConnection::start()
{
    if (_running) return;
    _running = true;
    _createSocket();
    _thread = std::thread(&ServerConnection::_loop, this);
}

void ServerConnection::stop()
{
    if (!_running) return;
    _running = false;
    if (_thread.joinable()) {
        _thread.join();
    }
    close(_fd);
    _fd = -1;
}

void ServerConnection::_loop()
{
    while (_running) {
        try {
            _receiveLoop();
            _sendLoop();
        } catch (const std::exception &e) {
            std::cerr << "[ServerConnection] Error in loop: " << e.what() << std::endl;
        }
    }
}

void ServerConnection::_receiveLoop()
{
}

void ServerConnection::_sendLoop()
{
}

Packet ServerConnection::_tryReceive()
{
}

void ServerConnection::_createSocket()
{
    if (_udp)
        _fd = socket(AF_INET, SOCK_DGRAM, 0);
    else
        _fd = socket(AF_INET, SOCK_STREAM, 0);

    if (_fd == -1) {
        throw std::runtime_error("Error creating socket");
    }

    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
    _addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(_fd, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
        throw std::runtime_error("Error binding socket");
    }

    if (!_udp) {
        if (listen(_fd, 0) < 0) {
            throw std::runtime_error("Error listening on TCP socket");
        }
    }
}

int ServerConnection::_selectFd()
{
    int retval;
    timeval tv = {1, 0};

    FD_ZERO(&_readfds);
    FD_ZERO(&_writefds);
    FD_SET(_fd, &_readfds);
    FD_SET(_fd, &_writefds);
    retval = select(_fd + 1, &_readfds, &_writefds, NULL, &tv);
    if (retval == -1) {
        throw std::runtime_error("Error selecting socket");
    }
    return retval;
}
