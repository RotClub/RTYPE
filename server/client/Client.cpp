//
// Created by Théo Bary on 10/12/2024.
//

#include "Client.hpp"
#include "spdlog/spdlog.h"
#include <cstring>
#include <stdexcept>
#include <Networking/Defines.hpp>
#include <random>

static std::string *generateUUID() {
    static std::random_device dev;
    static std::mt19937 rng(dev());

    std::uniform_int_distribution<int> dist(0, 15);

    const char *v = "0123456789ABCDEF";
    const bool dash[] = { 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0 };

    auto *res = new std::string("");
    for (int i = 0; i < 16; i++) {
        if (dash[i]) *res += "-";
        *res += v[dist(rng)];
        *res += v[dist(rng)];
    }
    return res;
}

Client::Client(const int srvTcpFd)
    : uuid(generateUUID())
{
    _step = ConnectionStep::UNVERIFIED;
    std::memset(&_address, 0, sizeof(_address));
    socklen_t len = sizeof(_address);
    _tcpFd = accept(srvTcpFd, reinterpret_cast<sockaddr *>(&_address), &len);
    if (_tcpFd == -1) {
        throw std::runtime_error("Error accepting client");
    }
    spdlog::debug("Client connected with UUID: {}", *uuid);
}

Client::~Client()
{
}

void Client::addTcpPacketInput(Packet *packet)
{
    std::get<IN>(_tcpQueues).enqueue(packet);
}

void Client::addUdpPacketInput(Packet *packet)
{
    std::get<IN>(_udpQueues).enqueue(packet);
}

Packet *Client::popTcpPacketInput()
{
    return std::get<IN>(_tcpQueues).dequeue();
}

Packet *Client::popUdpPacketInput()
{
    return std::get<IN>(_udpQueues).dequeue();
}

void Client::addTcpPacketOutput(Packet *packet)
{
    std::get<OUT>(_tcpQueues).enqueue(packet);
}

void Client::addUdpPacketOutput(Packet *packet)
{
    std::get<OUT>(_udpQueues).enqueue(packet);
}

Packet *Client::popTcpPacketOutput()
{
    return std::get<OUT>(_tcpQueues).dequeue();
}

Packet *Client::popUdpPacketOutput()
{
    return std::get<OUT>(_udpQueues).dequeue();
}

bool Client::hasTcpPacketOutput()
{
    return !std::get<OUT>(_tcpQueues).empty();
}

bool Client::hasUdpPacketOutput()
{
    return !std::get<OUT>(_udpQueues).empty();
}

bool Client::hasTcpPacketInput()
{
    return !std::get<IN>(_tcpQueues).empty();
}

bool Client::hasUdpPacketInput()
{
    return !std::get<IN>(_udpQueues).empty();
}
