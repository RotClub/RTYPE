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

#ifdef WIN32
    #include <ws2tcpip.h>
    #include <io.h>
#else
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <unistd.h>
#endif

#include <thread>
#include <vector>

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

bool ClientConnection::hasPendingTCPPacket() { return !std::get<PACKET_IN>(_tcpQueues).empty(); }

bool ClientConnection::hasPendingUDPPacket() { return !std::get<PACKET_IN>(_udpQueues).empty(); }

void ClientConnection::_tryReceiveTCP()
{
    std::vector<uint8_t> buffer(PACKED_PACKET_SIZE);
    int n = 0;
    if ((n = read(_tcpFd, buffer.data(), PACKED_PACKET_SIZE)) <= 0) {
        throw std::runtime_error("Disconnect");
    }
    buffer.resize(n);
    _tcpBuffer.insert(_tcpBuffer.end(), buffer.begin(), buffer.end());
}

void ClientConnection::_tryReceiveUDP()
{
    std::vector<uint8_t> buffer(PACKED_PACKET_SIZE);
    socklen_t len = sizeof(_addr);
    int n = 0;
    if ((n = recvfrom(_udpFd, reinterpret_cast<char *>(buffer.data()), PACKED_PACKET_SIZE, 0, reinterpret_cast<sockaddr *>(&_addr), &len)) <= 0) {
        throw std::runtime_error("Error receiving udp packet");
    }
    buffer.resize(n);
    _udpBuffer.insert(_udpBuffer.end(), buffer.begin(), buffer.end());
}

void ClientConnection::sendToServerTCP(Packet *packet)
{
    std::memcpy(packet->id, _id, sizeof(char[16]));
    std::get<PACKET_OUT>(_tcpQueues).enqueue(packet);
}

void ClientConnection::sendToServerUDP(Packet *packet)
{
    std::memcpy(packet->id, _id, sizeof(char[16]));
    std::get<PACKET_OUT>(_udpQueues).enqueue(packet);
}

void ClientConnection::_receiveLoop()
{
    if (FD_ISSET(_tcpFd, &_readfds)) {
        try {
            _tryReceiveTCP();
        }
        catch (const std::runtime_error &e) {
            Client::GetInstance().disconnectFromServer();
            return;
        }
        if (_tcpBuffer.size() >= PACKED_PACKET_SIZE) {
            Packet *packet = new Packet;
            PacketBuilder::PackedPacket packed = {0};
            try {
                std::memcpy(packed, _tcpBuffer.data(), PACKED_PACKET_SIZE);
                PacketBuilder::unpack(&packed, packet);
            } catch (const std::exception &e) {
                delete packet;
                return;
            }
            _tcpBuffer.erase(_tcpBuffer.begin(), _tcpBuffer.begin() + PACKED_PACKET_SIZE);
            if (packet)
                std::get<PACKET_IN>(_tcpQueues).enqueue(packet);
        }
    }
    if (FD_ISSET(_udpFd, &_readfds)) {
        try {
            _tryReceiveUDP();
        }
        catch (const std::runtime_error &e) {
            return;
        }
        if (_udpBuffer.size() >= PACKED_PACKET_SIZE) {
            Packet *packet = new Packet;
            PacketBuilder::PackedPacket packed = {0};
            try {
                std::memcpy(packed, _udpBuffer.data(), PACKED_PACKET_SIZE);
                PacketBuilder::unpack(&packed, packet);
            }
            catch (const std::exception &e) {
                delete packet;
                return;
            }
            _udpBuffer.erase(_udpBuffer.begin(), _udpBuffer.begin() + PACKED_PACKET_SIZE);
            if (packet)
                std::get<PACKET_IN>(_udpQueues).enqueue(packet);
        }
    }
}

void ClientConnection::_sendLoop()
{
    if (!_connected)
        return;
    if (FD_ISSET(_tcpFd, &_writefds)) {
        if (!std::get<PACKET_OUT>(_tcpQueues).empty()) {
            Packet *packet = std::get<PACKET_OUT>(_tcpQueues).dequeue();
            PacketBuilder::PackedPacket packed = {0};
            PacketBuilder::pack(&packed, packet);
            write(_tcpFd, packed, sizeof(PacketBuilder::PackedPacket));
            PacketBuilder(packet).reset();
            delete packet;
        }
    }
    if (FD_ISSET(_udpFd, &_writefds)) {
        if (!std::get<PACKET_OUT>(_udpQueues).empty()) {
            Packet *packet = std::get<PACKET_OUT>(_udpQueues).dequeue();
            PacketBuilder::PackedPacket packed = {0};
            PacketBuilder::pack(&packed, packet);
            sendto(_udpFd, packed, sizeof(PacketBuilder::PackedPacket), 0, reinterpret_cast<sockaddr *>(&_addr), sizeof(sockaddr_in));
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
