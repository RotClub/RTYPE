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
    if (read(_fd, packet, sizeof(Packet)) <= 0) {
        throw std::runtime_error("Disconnect");
    }
    spdlog::info("Packet size: {}", packet->n);
    packet->data = std::malloc(packet->n);
    if (read(_fd, packet->data, packet->n) <= 0) {
        throw std::runtime_error("Disconnect");
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
    try {
        _packet = _tryReceive();
    } catch (const std::runtime_error &e) {
        spdlog::info(e.what());
        _connected = false;
        return;
    }
    if (!_packet)
        return;
    std::get<IN>(_queues).enqueue(_packet);
    _packet = nullptr;
}

void ClientConnection::_sendLoop()
{
    if (!_connected)
        return;
    if (FD_ISSET(_fd, &_writefds) == 0)
        return;
    while (!std::get<OUT>(_queues).empty()) {
        Packet *sending = std::get<OUT>(_queues).dequeue();
        write(_fd, sending, sizeof(*sending));
        write(_fd, sending->data, sending->n);
        std::free(sending->data);
        delete sending;
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