/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** ServerConnection
*/

#include "ClientConnection.hpp"

#include "Engine.hpp"
#include "Networking/Packet.hpp"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"
#include <cstddef>
#include <unistd.h>

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
    return true;
}

Packet ClientConnection::_tryReceive()
{
    Packet pckt = NULL_PACKET;
    int dataSize = 0;

    if (read(_fd, &dataSize, sizeof(size_t)) <= 0) {
        return NULL_PACKET;
    }
    pckt.n = dataSize;
    pckt.cmd = PacketCmd::NONE;
    pckt.data = malloc(dataSize);
    if (read(_fd, &pckt, sizeof(PacketCmd) + dataSize) <= 0) {
        return NULL_PACKET;
    }
    return pckt;
}

void ClientConnection::sendToServer(Packet *pckt)
{
    std::get<OUT>(_queues).enqueue(pckt);
}

void ClientConnection::_receiveLoop()
{
    Packet receivedPacket = _tryReceive();
    _packet = &receivedPacket;
    if (!_packet)
        return;
    std::get<IN>(_queues).enqueue(_packet);
    _packet = nullptr;
}

void ClientConnection::_sendLoop()
{
    while (!std::get<OUT>(_queues).empty()) {
        Packet *sending = std::get<OUT>(_queues).dequeue();
        write(_fd, sending, sending->n);
        free(sending->data);
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