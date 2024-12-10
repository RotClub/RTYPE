/*
** EPITECH PROJECT, 2024
** Rtype [WSL: Ubuntu]
** File description:
** ServerConnection
*/

#include "ServerConnection.hpp"

ServerConnection::ServerConnection(int port)
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
    close(_tcpFd);
    close(_udpFd);
    _tcpFd = -1;
    _udpFd = -1;
}

void ServerConnection::_loop()
{
    while (_running) {
        try {
            _selectFd();
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
    _udpFd = socket(AF_INET, SOCK_DGRAM, 0);
    _tcpFd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&_addr, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
    _addr.sin_addr.s_addr = INADDR_ANY;

    if (_udpFd == -1 || _tcpFd == -1) {
        throw std::runtime_error("Error creating socket");
    }

    if (bind(_tcpFd, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
        throw std::runtime_error("Error binding tcp socket");
    }

    listen(_tcpFd, 0);

    if (bind(_udpFd, (struct sockaddr*)&_addr, sizeof(_addr)) < 0) {
        throw std::runtime_error("Error binding udp socket");
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
