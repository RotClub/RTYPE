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
    int new_socket;

    while (_running) {
        try {
            FD_ZERO(&_readfds);
            FD_SET(_fd, &_readfds);
            _max_sd = _fd;

            for (int sock : _clientSockets) {
                FD_SET(sock, &_readfds);
                if (sock > _max_sd)
                    _max_sd = sock;
            }

            int sel = select(_max_sd + 1, &_readfds, NULL, NULL, NULL);
            if (sel < 0 && errno != EINTR) {
                throw std::runtime_error("Error in select");
            }

            if (FD_ISSET(_fd, &_readfds)) {
                if (( 
            }

        } catch (const std::exception &e) {
            std::cerr << "[ServerConnection] Error in loop: " << e.what() << std::endl;
        }
    }
}

void ServerConnection::_receiveLoop()
{
    int sel;
    sel = _selectFd();
    if (sel == 0)
        return;
    _packet = _tryReceive();
    if (_packet.size == 0)
        return;
    std::get<IN>(_queues)->enqueue(_packet);
    _packet = NULL_PACKET;
}

void ServerConnection::_sendLoop()
{
    while (!std::get<OUT>(_queues)->empty()) {
        Packet sending = std::get<OUT>(_queues)->dequeue();
        uint32_t dataSize = sending.size;
        size_t bytesSent = 0;

        while (bytesSent < sizeof(dataSize)) {
            int result = send(_fd, reinterpret_cast<char*>(&dataSize) + bytesSent, sizeof(dataSize) - bytesSent, 0);
            if (result == -1)
                throw std::runtime_error("Error sending data size to client");
            bytesSent += result;
        }

        bytesSent = 0;
        while (bytesSent < dataSize) {
            int result = send(_fd, sending.data.data() + bytesSent, dataSize - bytesSent, 0);
            if (result == -1)
                throw std::runtime_error("Error sending data to client");
            bytesSent += result;
        }
    }
}

Packet ServerConnection::_tryReceive()
{
    uint32_t dataSize = 0;
    size_t bytesRead = 0;

    while (bytesRead < sizeof(dataSize)) {
        int result = read(_fd, reinterpret_cast<char*>(&dataSize) + bytesRead, sizeof(dataSize) - bytesRead);
        if (result == -1)
            throw std::runtime_error("Error reading data size from client");
        if (result == 0)
            return NULL_PACKET;
        bytesRead += result;
    }
    std::vector<void *> buffer(dataSize);
    bytesRead = 0;
    while (bytesRead < dataSize) {
        int result = read(_fd, buffer.data() + bytesRead, dataSize - bytesRead);
        if (result == -1)
            throw std::runtime_error("Error reading data from client");
        if (result == 0)
            return NULL_PACKET;
        bytesRead += result;
    }
    return (Packet){dataSize, buffer};
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
        if (listen(_fd, SOMAXCONN) < 0) {
            throw std::runtime_error("Error listening on TCP socket");
        }
    }
}
