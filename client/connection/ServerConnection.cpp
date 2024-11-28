/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** ServerConnection
*/

#include "ServerConnection.hpp"

ServerConnection::ServerConnection(std::string ip, int port, std::tuple<SafeQueue<std::string> *, SafeQueue<std::string> *> queues)
    : _ip(ip), _port(port), _queues(queues)
{
}

ServerConnection::~ServerConnection()
{
    disconnectFromServer();
}

void ServerConnection::connectToServer()
{
    _fd = socket(AF_INET, SOCK_STREAM, 0);
    if (_fd == -1) {
        throw std::runtime_error("Error creating socket");
    }
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
    _addr.sin_addr.s_addr = inet_addr(_ip.c_str());
    if (connect(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1) {
        throw std::runtime_error("Error connecting to server");
    }
    _connected = true;
    _thread = std::thread(&ServerConnection::_serverLoop, this);
}

void ServerConnection::disconnectFromServer()
{
    if (!_connected) {
        return;
    }
    std::cout << "Disconnecting..." << std::endl;
    _connected = false;
    try {
        _thread.join();
    } catch (const std::exception &e) {
        std::cerr << "Thread Error: " + std::string(e.what()) << std::endl;
    }
    close(_fd);
    _fd = -1;
}

std::string ServerConnection::tryReceive()
{
    char buffer[1024];
    int valread = 0;
    memset(buffer, 0, 1024);
    if (FD_ISSET(_fd, &_readfds)) {
        valread = read(_fd, buffer, 1024);
        if (valread == -1) {
            throw std::runtime_error("Error reading from server");
        }
        if (valread == 0) {
            return "";
        }
        return std::string(buffer);
    }
    return "";
}

void ServerConnection::sendToServer(std::string msg, std::string endWith)
{
    std::get<OUT>(_queues)->enqueue(msg + endWith);
}

int ServerConnection::_selectFd() {
    int retval;
    timeval tv = {1, 0};

    FD_ZERO(&_readfds);
    FD_SET(_fd, &_readfds);
    retval = select(_fd + 1, &_readfds, NULL, NULL, &tv);
    if (retval == -1) {
        throw std::runtime_error("Error selecting socket");
    }
    return retval;
}

void ServerConnection::_receiveLoop()
{
    int sel;
    sel = _selectFd();
    if (sel == 0) {
        return;
    }
    _buffer += tryReceive();
    while (_buffer.find("\n") != std::string::npos) {
        std::get<IN>(_queues)->enqueue(_buffer.substr(0, _buffer.find("\n")));
        std::cout << "<- : " << _buffer.substr(0, _buffer.find("\n")) << std::endl;
        _buffer = _buffer.substr(_buffer.find("\n") + 1);
    }
}

void ServerConnection::_sendLoop()
{
    std::string sending;

    while (!std::get<OUT>(_queues)->empty()) {
        sending = std::get<OUT>(_queues)->dequeue();
        if (send(_fd, sending.c_str(), sending.length(), 0) == -1) {
            throw std::runtime_error("Error sending to server");
        }
        std::cout << "-> : " << sending << std::endl;
    }
}

void ServerConnection::_serverLoop()
{
    while (_connected) {
        _receiveLoop();
        _sendLoop();
    }
}