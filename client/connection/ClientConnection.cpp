/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** ServerConnection
*/

#include "ClientConnection.hpp"

#include "Networking/Packet.hpp"
#include "Networking/PacketBuilder.hpp"
#include "spdlog/spdlog.h"
#include <stdexcept>
#include <sys/select.h>
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

void ClientConnection::establishConnection()
{
    PacketBuilder builder;
    builder.setCmd(PacketCmd::CONNECT);
    sendToServer(builder.build());
}

bool ClientConnection::hasPendingPacket()
{
    return !std::get<IN>(_queues).empty();
}

Packet *ClientConnection::_tryReceive()
{
    Packet *packet = new Packet;

    std::memset(packet, 0, sizeof(Packet));
    unsigned short tmpCmd = -1;
    if (read(_fd, &tmpCmd, sizeof(unsigned short)) <= 0) {
        throw std::runtime_error("Disconnect");
    }
    packet->cmd = static_cast<PacketCmd>(tmpCmd);
    if (read(_fd, &packet->n, sizeof(size_t)) <= 0) {
        throw std::runtime_error("Disconnect");
    }
    if (packet->n > 0) {
        packet->data = std::malloc(packet->n);
        if (read(_fd, packet->data, packet->n) <= 0) {
            throw std::runtime_error("Disconnect");
        }
    } else {
        packet->data = nullptr;
    }
    return packet;
}

void ClientConnection::sendToServer(Packet *pckt)
{
    std::get<OUT>(_queues).enqueue(pckt);
}

void ClientConnection::_receiveLoop()
{
    if (FD_ISSET(_fd, &_readfds) == 0) {
        return;
    }
    Packet *packet = nullptr;
    try {
        packet = _tryReceive();
    } catch (const std::runtime_error &e) {
        spdlog::info(e.what());
        _connected = false;
        return;
    }
    if (!packet)
        return;
    std::get<IN>(_queues).enqueue(packet);
    spdlog::debug("Queue size: {}", std::get<IN>(_queues).empty());
}

void ClientConnection::_sendLoop()
{
    if (!_connected)
        return;
    if (FD_ISSET(_fd, &_writefds) == 0)
        return;
    while (!std::get<OUT>(_queues).empty()) {
        Packet *packet = std::get<OUT>(_queues).dequeue();
        auto tmpCmd = static_cast<unsigned short>(packet->cmd);
        write(_fd, &tmpCmd, sizeof(unsigned short));
        write(_fd, &packet->n, sizeof(size_t));
        if (packet->n > 0) {
            write(_fd, packet->data, packet->n);
            std::free(packet->data);
        }
        delete packet;
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