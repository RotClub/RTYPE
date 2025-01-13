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

ClientConnection::ClientConnection(const std::string &ip, int port)
    : _ip(ip), _port(port)
{
}

ClientConnection::~ClientConnection()
{
    disconnectFromServer();
}

void ClientConnection::connectToServer()
{
    _createSocket();
    _addr.sin_port = htons(_port);
    _addr.sin_addr.s_addr = inet_addr(_ip.c_str());
    if (connect(_tcpFd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) == -1) {
        throw std::runtime_error("Error connecting to server via TCP");
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
    pthread_kill(_thread.native_handle(), SIGKILL);
    try {
        if (_thread.joinable())
            _thread.join();
    } catch (const std::exception &e) {
        std::cerr << "Thread Error: " + std::string(e.what()) << std::endl;
    }
    close(_tcpFd);
    close(_udpFd);
    _tcpFd = -1;
    _udpFd = -1;
}

void ClientConnection::establishConnection()
{
    PacketBuilder builder;
    builder.setCmd(PacketCmd::CONNECT);
    sendToServerTCP(builder.build());
}

bool ClientConnection::hasPendingTCPPacket()
{
    return !std::get<IN>(_tcpQueues).empty();
}

bool ClientConnection::hasPendingUDPPacket()
{
    return !std::get<IN>(_udpQueues).empty();
}

Packet *ClientConnection::_tryReceiveTCP()
{
    Packet *packet = new Packet;

    std::memset(packet, 0, sizeof(Packet));
    unsigned short tmpCmd = -1;
    if (read(_tcpFd, &tmpCmd, sizeof(unsigned short)) <= 0) {
        return nullptr;
    }
    packet->cmd = static_cast<PacketCmd>(tmpCmd);
    if (read(_tcpFd, &packet->n, sizeof(size_t)) <= 0) {
        return nullptr;
    }
    if (packet->n > 0) {
        packet->data = std::malloc(packet->n);
        if (read(_tcpFd, packet->data, packet->n) <= 0) {
            return nullptr;
        }
    } else {
        packet->data = nullptr;
    }
    return packet;
}

Packet* ClientConnection::_tryReceiveUDP()
{
    Packet *packet = new Packet;

    std::memset(packet, 0, sizeof(Packet));
    unsigned short tmpCmd = -1;
    socklen_t len = sizeof(_addr);
    if (recvfrom(_udpFd, &tmpCmd, sizeof(unsigned short), 0, reinterpret_cast<sockaddr *>(&_addr), &len) <= 0) {
        return nullptr;
    }
    packet->cmd = static_cast<PacketCmd>(tmpCmd);
    if (recvfrom(_udpFd, &packet->n, sizeof(size_t), 0, reinterpret_cast<sockaddr *>(&_addr), &len) <= 0) {
        return nullptr;
    }
    if (packet->n > 0) {
        packet->data = std::malloc(packet->n);
        if (recvfrom(_udpFd, packet->data, packet->n, 0, reinterpret_cast<sockaddr *>(&_addr), &len) <= 0) {
            return nullptr;
        }
    } else {
        packet->data = nullptr;
    }
    return packet;
}

void ClientConnection::sendToServerTCP(Packet *packet)
{
    std::memcpy(packet->id, _id, sizeof(char[16]));
    std::get<OUT>(_tcpQueues).enqueue(packet);
}

void ClientConnection::sendToServerUDP(Packet *packet)
{
    std::memcpy(packet->id, _id, sizeof(char[16]));
    std::get<OUT>(_udpQueues).enqueue(packet);
}

void ClientConnection::_receiveLoop()
{
    if (FD_ISSET(_tcpFd, &_readfds)) {
        Packet *packet = nullptr;
        try {
            packet = _tryReceiveTCP();
        } catch (const std::runtime_error &e) {
            spdlog::error(e.what());
        }
        if (packet)
            std::get<IN>(_tcpQueues).enqueue(packet);
    }
    if (FD_ISSET(_udpFd, &_readfds)) {
        Packet *packet = nullptr;
        try {
            packet = _tryReceiveUDP();
        } catch (const std::runtime_error &e) {
            spdlog::error(e.what());
        }
        if (packet)
            std::get<IN>(_udpQueues).enqueue(packet);
    }
}

void ClientConnection::_sendLoop()
{
    if (!_connected)
        return;
    if (FD_ISSET(_tcpFd, &_writefds)) {
        while (!std::get<OUT>(_tcpQueues).empty()) {
            Packet *packet = std::get<OUT>(_tcpQueues).dequeue();
            auto tmpCmd = static_cast<unsigned short>(packet->cmd);
            write(_tcpFd, &tmpCmd, sizeof(unsigned short));
            write(_tcpFd, &packet->id, sizeof(char[16]));
            write(_tcpFd, &packet->n, sizeof(size_t));
            if (packet->n > 0) {
                write(_tcpFd, packet->data, packet->n);
            }
            PacketBuilder(packet).reset();
            delete packet;
        }
    }
    if (FD_ISSET(_udpFd, &_writefds)) {
        while (!std::get<OUT>(_udpQueues).empty()) {
            Packet *packet = std::get<OUT>(_udpQueues).dequeue();
            auto tmpCmd = static_cast<unsigned short>(packet->cmd);
            sendto(_udpFd, &tmpCmd, sizeof(unsigned short), 0, reinterpret_cast<sockaddr *>(&_addr), sizeof(sockaddr_in));
            sendto(_udpFd, &packet->id, sizeof(char[16]), 0, reinterpret_cast<sockaddr *>(&_addr), sizeof(sockaddr_in));
            sendto(_udpFd, &packet->n, sizeof(size_t), 0, reinterpret_cast<sockaddr *>(&_addr), sizeof(sockaddr_in));
            if (packet->n > 0) {
                sendto(_udpFd, packet->data, packet->n, 0, reinterpret_cast<sockaddr *>(&_addr), sizeof(sockaddr_in));
            }
            PacketBuilder(packet).reset();
            delete packet;
        }
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