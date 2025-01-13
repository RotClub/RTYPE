/*
** EPITECH PROJECT, 2024
** Rtype [WSL: Ubuntu]
** File description:
** ServerConnection
*/

#include "ServerConnection.hpp"

#include <Engine.hpp>
#include <cstddef>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

ServerConnection::ServerConnection(int port)
    : _port(port)
{
}

ServerConnection::~ServerConnection()
{
    stop();
}

void ServerConnection::start()
{
    if (_running) return;
    _running = true;
    _createSocket();
    _networkThread = std::thread(&ServerConnection::_loop, this);
}

void ServerConnection::stop()
{
    if (!_running) return;
    _running = false;
    pthread_cancel(_networkThread.native_handle());
    if (_networkThread.joinable()) {
        _networkThread.join();
    }
    for (const auto *client : _clientConnections) {
        delete client;
    }
    close(_tcpFd);
    close(_udpFd);
    _tcpFd = -1;
    _udpFd = -1;
}

void ServerConnection::_loop()
{
    while (_running) {
        try {
            if (_selectFd() == -1)
                spdlog::error(std::strerror(errno));
            _accept();
            _receiveLoop();
            _disconnectClients();
            _sendLoop();
        } catch (const std::exception &e) {
            spdlog::error("Error in networking loop: {}", e.what());
        }
    }
}

void ServerConnection::_disconnectClients()
{
    auto it = std::ranges::remove_if(_clientConnections, [](Client *client) {
        return client->shouldDisconnect();
    }).begin();
    if (it != _clientConnections.end()) {
        Client *client = *it;
        _clientConnections.erase(it, _clientConnections.end());
        delete client;
    }
}

void ServerConnection::_receiveLoop()
{
    for (auto &client : _clientConnections) {
        if (!client->shouldDisconnect() && FD_ISSET(client->getTcpFd(), &_readfds)) {
            try {
                Packet *packet = _tryReceiveTCP(client);
                client->addTcpPacketInput(packet);
            } catch (const std::exception &e) {
                client->disconnect();
            }
        }
    }
    if (FD_ISSET(_udpFd, &_readfds)) {
        sockaddr_in addr{};
        std::memset(&addr, 0, sizeof(addr));
        Packet *packet = _tryReceiveUDP(&addr);
        Client *client = _getClientByID(packet->id);
        if (client == nullptr) {
            PacketBuilder(packet).reset();
            delete packet;
            return;
        }
        client->updateUdpAddress(&addr);
        if (!client->shouldDisconnect())
            client->addUdpPacketInput(packet);
    }
}

void ServerConnection::_sendLoop()
{
    for (auto &client : _clientConnections) {
        if (!client->shouldDisconnect()) {
            if (client->hasTcpPacketOutput() && FD_ISSET(client->getTcpFd(), &_writefds)) {
                Packet *packet = client->popTcpPacketOutput();
                auto tmpCmd = static_cast<unsigned short>(packet->cmd);
                write(client->getTcpFd(), &tmpCmd, sizeof(unsigned short));
                write(client->getTcpFd(), &packet->n, sizeof(size_t));
                if (packet->n > 0) {
                    write(client->getTcpFd(), packet->data, packet->n);
                }
                PacketBuilder(packet).reset();
                delete packet;
            }
            if (client->hasUdpPacketOutput()) {
                Packet *packet = client->popUdpPacketOutput();
                auto tmpCmd = static_cast<unsigned short>(packet->cmd);
                sendto(_udpFd, &tmpCmd, sizeof(unsigned short), 0, reinterpret_cast<sockaddr *>(client->getUdpAddress()), sizeof(sockaddr_in));
                sendto(_udpFd, &packet->n, sizeof(size_t), 0, reinterpret_cast<sockaddr *>(client->getUdpAddress()), sizeof(sockaddr_in));
                if (packet->n > 0) {
                    sendto(_udpFd, packet->data, packet->n, 0, reinterpret_cast<sockaddr *>(client->getUdpAddress()), sizeof(sockaddr_in));
                }
                PacketBuilder(packet).reset();
                delete packet;
            }
        }
    }
}

void ServerConnection::_accept()
{
    if (FD_ISSET(_tcpFd, &_readfds)) {
        _clientConnections.emplace_back(new Client(_tcpFd));
    }
}

Packet *ServerConnection::_tryReceiveTCP(Client *client)
{
    Packet *packet = new Packet;

    std::memset(packet, 0, sizeof(Packet));
    unsigned short tmpCmd = -1;
    if (read(client->getTcpFd(), &tmpCmd, sizeof(unsigned short)) <= 0) {
        throw std::runtime_error("Disconnect");
    }
    packet->cmd = static_cast<PacketCmd>(tmpCmd);
    if (read(client->getTcpFd(), &packet->id, sizeof(char[16])) <= 0) {
        throw std::runtime_error("Disconnect");
    }
    if (read(client->getTcpFd(), &packet->n, sizeof(size_t)) <= 0) {
        throw std::runtime_error("Disconnect");
    }
    if (packet->n > 0) {
        packet->data = std::malloc(packet->n);
        if (read(client->getTcpFd(), packet->data, packet->n) <= 0) {
            throw std::runtime_error("Disconnect");
        }
    } else {
        packet->data = nullptr;
    }
    return packet;
}

Packet *ServerConnection::_tryReceiveUDP(sockaddr_in *addr)
{
    Packet *packet = new Packet;

    std::memset(packet, 0, sizeof(Packet));
    unsigned short tmpCmd = -1;
    socklen_t len = sizeof(*addr);
    if (recvfrom(_udpFd, &tmpCmd, sizeof(unsigned short), 0, reinterpret_cast<sockaddr *>(addr), &len) <= 0) {
        throw std::runtime_error("Error receiving udp packet");
    }
    packet->cmd = static_cast<PacketCmd>(tmpCmd);
    if (recvfrom(_udpFd, &packet->id, sizeof(char[16]), 0, reinterpret_cast<sockaddr *>(addr), &len) <= 0) {
        throw std::runtime_error("Error receiving udp packet");
    }
    if (recvfrom(_udpFd, &packet->n, sizeof(size_t), 0, reinterpret_cast<sockaddr *>(addr), &len) <= 0) {
        throw std::runtime_error("Error receiving udp packet");
    }
    if (packet->n > 0) {
        packet->data = std::malloc(packet->n);
        if (recvfrom(_udpFd, packet->data, packet->n, 0, reinterpret_cast<sockaddr *>(addr), &len) <= 0) {
            throw std::runtime_error("Error receiving udp packet");
        }
    } else {
        packet->data = nullptr;
    }
    return packet;
}

void ServerConnection::_createSocket()
{
    _udpFd = socket(AF_INET, SOCK_DGRAM, 0);
    _tcpFd = socket(AF_INET, SOCK_STREAM, 0);

    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
    _addr.sin_addr.s_addr = INADDR_ANY;
    int optval = 1;
    setsockopt(_tcpFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    setsockopt(_udpFd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

    if (_udpFd == -1 || _tcpFd == -1) {
        throw std::runtime_error("Error creating socket");
    }

    if (bind(_tcpFd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) < 0) {
        throw std::runtime_error("Error binding tcp socket");
    }

    listen(_tcpFd, 0);

    if (bind(_udpFd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) < 0) {
        throw std::runtime_error("Error binding udp socket");
    }
}

void ServerConnection::_setClientFds(fd_set *set)
{
    FD_ZERO(set);

    for (const auto &client : _clientConnections) {
        FD_SET(client->getTcpFd(), set);
    }
}

int ServerConnection::_getMaxFd()
{
    int max = 0;

    for (const auto &client : _clientConnections) {
        if (client->shouldDisconnect())
            continue;
        max = std::max(max, client->getTcpFd());
    }
    return max;
}

int ServerConnection::_selectFd()
{
    int retval;

    _setClientFds(&_readfds);
    _setClientFds(&_writefds);
    FD_SET(_tcpFd, &_readfds);
    FD_SET(_udpFd, &_readfds);
    int maxFd = std::max(std::max(_getMaxFd(), _tcpFd), _udpFd);
    retval = select(maxFd + 1, &_readfds, &_writefds, nullptr, nullptr);
    return retval;
}

Client* ServerConnection::_getClientByID(const char id[16])
{
    for (auto client : _clientConnections) {
        if (std::memcmp(client->getID(), id, sizeof(char[16])) == 0) {
            return client;
        }
    }
    return nullptr;
}
