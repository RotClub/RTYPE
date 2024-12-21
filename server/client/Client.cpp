//
// Created by Théo Bary on 10/12/2024.
//

#include "Client.hpp"
#include <cstring>
#include <stdexcept>
#include <Networking/Defines.hpp>

Client::Client(const int srvTcpFd)
{
    _step = ConnectionStep::UNVERIFIED;
    std::memset(&_address, 0, sizeof(_address));
    socklen_t len = sizeof(_address);
    _tcpFd = accept(srvTcpFd, reinterpret_cast<sockaddr *>(&_address), &len);
    if (_tcpFd == -1) {
        throw std::runtime_error("Error accepting client");
    }
}

Client::~Client()
{
}

void Client::addTcpPacketInput(Packet *packet)
{
    std::get<IN>(_tcpQueues).enqueue(packet);
}

Packet *Client::popTcpPacketInput()
{
    return std::get<IN>(_tcpQueues).dequeue();
}

void Client::addTcpPacketOutput(Packet *packet)
{
    std::get<OUT>(_tcpQueues).enqueue(packet);
}

Packet *Client::popTcpPacketOutput()
{
    return std::get<OUT>(_tcpQueues).dequeue();
}

bool Client::hasTcpPacketOutput()
{
    return !std::get<OUT>(_tcpQueues).empty();
}
