//
// Created by Théo Bary on 10/12/2024.
//

#include "Client.hpp"

#include <Engine.hpp>
#include <Networking/Defines.hpp>
#include <cstring>
#include <random>
#include <stdexcept>
#include <unistd.h>

#ifdef WIN32
#include <WindowsCross.hpp>
#endif
#include "spdlog/spdlog.h"

static std::string generateUUID()
{
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789ABCDEF";

    std::string str;
    for (int i = 0; i < 16; i++) {
        str += v[dist(rng)];
    }
    return str;
}

Client::Client(const int srvTcpFd) : uuid(generateUUID()), _shouldDisconnect(false)
{
    _step = ConnectionStep::UNVERIFIED;
    std::memset(&_address, 0, sizeof(_address));
    std::memset(&_udpAddress, 0, sizeof(_udpAddress));
    socklen_t len = sizeof(_address);
    _tcpFd = accept(srvTcpFd, reinterpret_cast<sockaddr *>(&_address), &len);
    if (_tcpFd == -1) {
        throw std::runtime_error("Error accepting client");
    }
    std::string tmpID = generateUUID();
    std::memcpy(_id, tmpID.data(), sizeof(char[16]));
}

Client::~Client()
{
    close(_tcpFd);
    spdlog::info("Client {} disconnected", uuid);
}

void Client::addTcpPacketInput(Packet *packet) { std::get<PACKET_IN>(_tcpQueues).enqueue(packet); }

void Client::addUdpPacketInput(Packet *packet) { std::get<PACKET_IN>(_udpQueues).enqueue(packet); }

Packet *Client::popTcpPacketInput() { return std::get<PACKET_IN>(_tcpQueues).dequeue(); }

Packet *Client::popUdpPacketInput() { return std::get<PACKET_IN>(_udpQueues).dequeue(); }

void Client::addTcpPacketOutput(Packet *packet)
{
    std::get<PACKET_OUT>(_tcpQueues).enqueue(packet);
}

void Client::addUdpPacketOutput(Packet *packet)
{
    std::get<PACKET_OUT>(_udpQueues).enqueue(packet);
}

Packet *Client::popTcpPacketOutput() { return std::get<PACKET_OUT>(_tcpQueues).dequeue(); }

Packet *Client::popUdpPacketOutput() { return std::get<PACKET_OUT>(_udpQueues).dequeue(); }

bool Client::hasTcpPacketOutput() { return !std::get<PACKET_OUT>(_tcpQueues).empty(); }

bool Client::hasUdpPacketOutput() { return !std::get<PACKET_OUT>(_udpQueues).empty(); }

bool Client::hasTcpPacketInput() { return !std::get<PACKET_IN>(_tcpQueues).empty(); }

bool Client::hasUdpPacketInput() { return !std::get<PACKET_IN>(_udpQueues).empty(); }
