/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** ServerConnection
*/

#include "ClientConnection.hpp"

#include "Engine.hpp"

ClientConnection::ClientConnection(std::string ip, int port, bool udp)
    : GlobalConnection(udp), _ip(ip), _port(port)
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
    // std::string keyword = KEYWORD;
    // Packet *pckt = PacketBuilder().writeString(keyword).build();
    // sendToServer(pckt);
    // Packet *response = getLatestPacket();
    // if (response->size == 0)
    //     return false;
    return true;
}

Packet *ClientConnection::_tryReceive()
{
    Packet *pckt = new Packet;
    int dataSize = 0;

    ssize_t ret = read(_fd, &dataSize, sizeof(int));
    if (ret == -1)
        throw std::runtime_error("Error reading data size from server");
    if (ret == 0 || dataSize == 0)
        return nullptr;
    Engine::GetInstance().Log(Engine::LogLevel::INFO, "Data size: " + std::to_string(dataSize));
    ssize_t ret2 = read(_fd, &pckt, dataSize);
    if (ret2 == -1)
        throw std::runtime_error("Error reading data from server");
    if (ret2 == 0)
        return nullptr;
    return pckt;
}

void ClientConnection::sendToServer(Packet *pckt)
{
    std::get<OUT>(_queues).enqueue(pckt);
}

void ClientConnection::_receiveLoop()
{
    _packet = _tryReceive();
    if (!_packet)
        return;
    std::get<IN>(_queues).enqueue(_packet);
    _packet = nullptr;
}

void ClientConnection::_sendLoop()
{
    while (!std::get<OUT>(_queues).empty()) {
        Packet *sending = std::get<OUT>(_queues).dequeue();
        size_t dataSize = sizeof(sending);

        write(_fd, &dataSize, sizeof(size_t));
        write(_fd, sending->data.data(), dataSize);
    }
}

void ClientConnection::_loop()
{
    while (_connected) {
        int sel = _selectFd();
        if (sel == 0)
            continue;
        _receiveLoop();
        _sendLoop();
    }
}