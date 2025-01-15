/*
** EPITECH PROJECT, 2024
** Rtype [WSL: Ubuntu]
** File description:
** ServerConnection
*/

#include "ServerConnection.hpp"

#include <Engine.hpp>
#include <arpa/inet.h>
#include <cstddef>
#include <netinet/in.h>
#include <sys/socket.h>

ServerConnection::ServerConnection(int port) : _port(port) {}

ServerConnection::~ServerConnection() { stop(); }

void ServerConnection::start()
{
    if (_running)
        return;
    _running = true;
    _createSocket();
    _networkThread = std::thread(&ServerConnection::_loop, this);
}

void ServerConnection::stop()
{
    if (!_running)
        return;
    _running = false;
    pthread_cancel(_networkThread.native_handle());
    if (_networkThread.joinable()) {
        _networkThread.join();
    }
    for (const auto *client : _clientConnections) {
        _disconnectedClients.enqueue(client->getUuid());
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
        }
        catch (const std::exception &e) {
            spdlog::error("Error in networking loop: {}", e.what());
        }
    }
}

void ServerConnection::_disconnectClients()
{
    auto it =
        std::ranges::remove_if(_clientConnections, [](Client *client) { return client->shouldDisconnect(); }).begin();
    if (it != _clientConnections.end()) {
        Client *client = *it;
        _clientConnections.erase(it, _clientConnections.end());
        _disconnectedClients.enqueue(client->getUuid());
        delete client;
    }
}

void ServerConnection::_receiveLoop()
{
    for (auto &client : _clientConnections) {
        if (!client->shouldDisconnect() && FD_ISSET(client->getTcpFd(), &_readfds)) {
            try {
                Packet *packet = _tryReceiveTCP(client);
                if (packet == nullptr)
                    continue;
                client->addTcpPacketInput(packet);
            }
            catch (const std::exception &e) {
                client->disconnect();
            }
        }
    }
    if (FD_ISSET(_udpFd, &_readfds)) {
        sockaddr_in addr{};
        std::memset(&addr, 0, sizeof(addr));
        Packet *packet = _tryReceiveUDP(&addr);
        if (packet == nullptr)
            return;
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
                PacketBuilder::PackedPacket packed = {0};
                PacketBuilder::pack(&packed, packet);
                write(client->getTcpFd(), &packed, sizeof(PacketBuilder::PackedPacket));
                PacketBuilder(packet).reset();
                delete packet;
            }
            if (client->hasUdpPacketOutput()) {
                Packet *packet = client->popUdpPacketOutput();
                PacketBuilder::PackedPacket packed = {0};
                PacketBuilder::pack(&packed, packet);
                sendto(_udpFd, &packed, sizeof(PacketBuilder::PackedPacket), 0, reinterpret_cast<sockaddr *>(client->getUdpAddress()),
                       sizeof(sockaddr_in));
                PacketBuilder(packet).reset();
                delete packet;
            }
        }
    }
}

void ServerConnection::_accept()
{
    if (_clientConnections.size() >= Engine::GetInstance().getGameInfo()->getMaxPlayers())
        return;
    if (FD_ISSET(_tcpFd, &_readfds)) {
        _clientConnections.emplace_back(new Client(_tcpFd));
    }
}

Packet *ServerConnection::_tryReceiveTCP(Client *client)
{
    PacketBuilder::PackedPacket packed = {0};
    if (read(client->getTcpFd(), &packed, sizeof(PacketBuilder::PackedPacket)) <= 0) {
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

Packet *ServerConnection::_tryReceiveUDP(sockaddr_in *addr)
{
    PacketBuilder::PackedPacket packed = {0};

    socklen_t len = sizeof(*addr);
    if (recvfrom(_udpFd, &packed, sizeof(PacketBuilder::PackedPacket), 0, reinterpret_cast<sockaddr *>(addr), &len) <= 0) {
        throw std::runtime_error("Error receiving udp packet");
    }
    Packet *packet = new Packet;
    try {
        spdlog::debug("Received udp packet");
        PacketBuilder::unpack(&packed, packet);
    }
    catch (const std::exception &e) {
        delete packet;
        return nullptr;
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

Client *ServerConnection::_getClientByID(const char id[16])
{
    for (auto client : _clientConnections) {
        if (std::memcmp(client->getID(), id, sizeof(char[16])) == 0) {
            return client;
        }
    }
    return nullptr;
}
