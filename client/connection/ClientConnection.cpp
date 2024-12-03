/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** ServerConnection
*/

#include "ClientConnection.hpp"

ClientConnection::ClientConnection(std::string ip, int port, bool udp)
    : GlobalConnection(), _ip(ip), _port(port)
{
}

ClientConnection::~ClientConnection()
{
    disconnectFromServer();
}

void ClientConnection::connectToServer()
{
    _addr.sin_port = htons(_port);
    _addr.sin_addr.s_addr = inet_addr(_ip.c_str());
    if (connect(_fd, (struct sockaddr *)&_addr, sizeof(_addr)) == -1) {
        throw std::runtime_error("Error connecting to server");
    }
    _connected = true;
    _thread = std::thread(&ClientConnection::_loop, this);
}

void ClientConnection::disconnectFromServer()
{
    if (!_connected) {
        return;
    }
    _connected = false;
    try {
        _thread.join();
    } catch (const std::exception &e) {
        std::cerr << "Thread Error: " + std::string(e.what()) << std::endl;
    }
    close(_fd);
    _fd = -1;
}

bool ClientConnection::establishConnection()
{
    std::string keyword = KEYWORD;
    Packet pckt = PacketBuilder().writeString(keyword).build();
    sendToServer(pckt);
    Packet response = getLatestPacket();
    if (response.size == 0)
        return false;
    return true;
}

Packet ClientConnection::_tryReceive()
{
    uint32_t dataSize = 0;
    size_t bytesRead = 0;

    while (bytesRead < sizeof(dataSize)) {
        int result = read(_fd, reinterpret_cast<char*>(&dataSize) + bytesRead, sizeof(dataSize) - bytesRead);
        if (result == -1)
            throw std::runtime_error("Error reading data size from server");
        if (result == 0)
            return NULL_PACKET;
        bytesRead += result;
    }
    std::vector<void *> buffer(dataSize);
    bytesRead = 0;
    while (bytesRead < dataSize) {
        int result = read(_fd, buffer.data() + bytesRead, dataSize - bytesRead);
        if (result == -1)
            throw std::runtime_error("Error reading data from server");
        if (result == 0)
            return NULL_PACKET;
        bytesRead += result;
    }
    return (Packet){dataSize, buffer};
}

void ClientConnection::sendToServer(Packet pckt)
{
    std::get<OUT>(_queues)->enqueue(pckt);
}

void ClientConnection::_receiveLoop()
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

void ClientConnection::_sendLoop()
{
    while (!std::get<OUT>(_queues)->empty()) {
        Packet sending = std::get<OUT>(_queues)->dequeue();
        uint32_t dataSize = sending.size;
        size_t bytesSent = 0;

        while (bytesSent < sizeof(dataSize)) {
            int result = send(_fd, reinterpret_cast<char*>(&dataSize) + bytesSent, sizeof(dataSize) - bytesSent, 0);
            if (result == -1)
                throw std::runtime_error("Error sending data size to server");
            bytesSent += result;
        }

        bytesSent = 0;
        while (bytesSent < dataSize) {
            int result = send(_fd, sending.data.data() + bytesSent, dataSize - bytesSent, 0);
            if (result == -1)
                throw std::runtime_error("Error sending data to server");
            bytesSent += result;
        }
    }
}

void ClientConnection::_loop()
{
    while (_connected) {
        _receiveLoop();
        _sendLoop();
    }
}