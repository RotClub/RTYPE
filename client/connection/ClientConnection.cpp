/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** ServerConnection
*/

#include "ClientConnection.hpp"

#include <cstring>
#include <iostream>
#include <signal.h>
#include <stdexcept>
#include <sys/select.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include "Client.hpp"
#include "Networking/Packet.hpp"
#include "Networking/PacketBuilder.hpp"
#include "spdlog/spdlog.h"

ClientConnection::ClientConnection(const std::string &ip, int port) : _ip(ip), _port(port) {}

ClientConnection::~ClientConnection() { disconnectFromServer(); }

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
    }
    catch (const std::exception &e) {
        std::cerr << "Thread Error: " << e.what() << std::endl;
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

bool ClientConnection::hasPendingTCPPacket() { return !std::get<IN>(_tcpQueues).empty(); }

bool ClientConnection::hasPendingUDPPacket() { return !std::get<IN>(_udpQueues).empty(); }

Packet *ClientConnection::_tryReceiveTCP()
{
    PacketBuilder::PackedPacket packed = {0};
    if (read(_tcpFd, &packed, sizeof(PacketBuilder::PackedPacket)) <= 0) {
        throw std::runtime_error("Disconnect");
    }
    Packet *packet = new Packet;
    try {
        PacketBuilder::unpack(&packed, packet);
    }
    catch (const std::exception &e) {
        delete packet;
        return nullptr;
    }
    return packet;
}

Packet *ClientConnection::_tryReceiveUDP()
{
    PacketBuilder::PackedPacket packed = {0};

    socklen_t len = sizeof(_addr);
    if (recvfrom(_udpFd, &packed, sizeof(PacketBuilder::PackedPacket), 0, reinterpret_cast<sockaddr *>(&_addr), &len) <= 0) {
        throw std::runtime_error("Error receiving udp packet");
    }
    Packet *packet = new Packet;
    try {
        PacketBuilder::unpack(&packed, packet);
    }
    catch (const std::exception &e) {
        delete packet;
        return nullptr;
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
        }
        catch (const std::runtime_error &e) {
            Client::GetInstance().disconnectFromServer();
            return;
        }
        if (packet)
            std::get<IN>(_tcpQueues).enqueue(packet);
    }
    if (FD_ISSET(_udpFd, &_readfds)) {
        Packet *packet = nullptr;
        try {
            packet = _tryReceiveUDP();
        }
        catch (const std::runtime_error &e) {
            packet = nullptr;
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
        if (!std::get<OUT>(_tcpQueues).empty()) {
            Packet *packet = std::get<OUT>(_tcpQueues).dequeue();
            PacketBuilder::PackedPacket packed = {0};
            PacketBuilder::pack(&packed, packet);
            if (std::memcmp(packed, PacketBuilder::integrityChallenge.c_str(), sizeof(char) * 32) != 0) {
                spdlog::error("Invalid packet integrity challenge.");
                throw std::runtime_error("Invalid packet integrity challenge.");
            }
            write(_tcpFd, &packed, sizeof(PacketBuilder::PackedPacket));
            PacketBuilder(packet).reset();
            delete packet;
        }
    }
    if (FD_ISSET(_udpFd, &_writefds)) {
        if (!std::get<OUT>(_udpQueues).empty()) {
            Packet *packet = std::get<OUT>(_udpQueues).dequeue();
            PacketBuilder::PackedPacket packed = {0};
            PacketBuilder::pack(&packed, packet);
            sendto(_udpFd, &packed, sizeof(PacketBuilder::PackedPacket), 0, reinterpret_cast<sockaddr *>(&_addr), sizeof(sockaddr_in));
            PacketBuilder(packet).reset();
            delete packet;
        }
    }
}

void ClientConnection::_loop()
{
    while (_connected) {
        int sel = _selectFd();
        if (sel == -1)
            return;
        _receiveLoop();
        _sendLoop();
    }
}
