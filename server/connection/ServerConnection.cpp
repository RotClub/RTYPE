/*
** EPITECH PROJECT, 2024
** Rtype [WSL: Ubuntu]
** File description:
** ServerConnection
*/

#include "ServerConnection.hpp"

#include <Engine.hpp>
#include <csignal>
#ifdef WIN32
#include <WindowsCross.hpp>
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif
#include <cstddef>

ServerConnection::ServerConnection(int port) : _port(port)
{
#ifdef WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        throw std::runtime_error("Failed to initialize Winsock.\n");
    }
#endif
}

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
    if (_networkThread.joinable()) {
        _networkThread.join();
    }
    for (const auto *client : _clientConnections) {
        _disconnectedClients.enqueue(client->getUuid());
        delete client;
    }

#ifdef WIN32
    _close(_tcpFd);
    _close(_udpFd);
#else
    close(_tcpFd);
    close(_udpFd);
#endif

    _tcpFd = -1;
    _udpFd = -1;
#ifdef WIN32
    WSACleanup();
#endif
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
    std::lock_guard guard(_clientsMutex);
    if (auto it = std::find_if(_clientConnections.begin(), _clientConnections.end(), [](Client *client) { return client->shouldDisconnect(); }); it != _clientConnections.end()) {
        Client *client = *it;
        _clientConnections.erase(it, _clientConnections.end());
        _disconnectedClients.enqueue(client->getUuid());
        delete client;
    }
}

void ServerConnection::_receiveLoop()
{
#ifndef far
#define far
#endif
    for (auto &client : _clientConnections) {
        if (!client->shouldDisconnect() && FD_ISSET(client->getTcpFd(), &_readfds)) {
            try {
                _tryReceiveTCP(client);
            }
            catch (const std::exception &e) {
                client->disconnect();
            }
        }
    }
    if (FD_ISSET(_udpFd, &_readfds)) {
        _tryReceiveUDP();
    }
}

void ServerConnection::_sendLoop()
{
#ifndef far
#define far
#endif
    for (auto &client : _clientConnections) {
        if (!client->shouldDisconnect()) {
            if (client->hasTcpPacketOutput() && FD_ISSET(client->getTcpFd(), &_writefds)) {
                Packet *packet = client->popTcpPacketOutput();
                PacketBuilder::PackedPacket packed = {0};
                PacketBuilder::pack(&packed, packet);

#ifdef WIN32
                _write(client->getTcpFd(), packed, sizeof(PacketBuilder::PackedPacket));
#else
                write(client->getTcpFd(), packed, sizeof(PacketBuilder::PackedPacket));
#endif

                PacketBuilder(packet).reset();
                delete packet;
            }
            if (client->hasUdpPacketOutput()) {
                Packet *packet = client->popUdpPacketOutput();
                PacketBuilder::PackedPacket packed = {0};
                PacketBuilder::pack(&packed, packet);
                sendto(_udpFd, packed, sizeof(PacketBuilder::PackedPacket), 0, reinterpret_cast<sockaddr *>(client->getUdpAddress()),
                       sizeof(sockaddr_in));
                PacketBuilder(packet).reset();
                delete packet;
            }
        }
    }
}

void ServerConnection::_accept()
{
#ifndef far
#define far
#endif
    if (_clientConnections.size() >= Engine::GetInstance().getGameInfo()->getMaxPlayers())
        return;
    if (FD_ISSET(_tcpFd, &_readfds)) {
        std::lock_guard guard(_clientsMutex);
        _clientConnections.emplace_back(new Client(_tcpFd));
    }
}

void ServerConnection::_tryReceiveTCP(Client *client)
{
    std::vector<uint8_t> buffer(PACKED_PACKET_SIZE);
    int n = 0;
#ifdef WIN32
    if ((n = recv(_tcpFd, reinterpret_cast<char *>(buffer.data()), PACKED_PACKET_SIZE, 0)) <= 0) {
        throw std::runtime_error("Disconnect");
    }
#else
    if ((n = read(client->getTcpFd(), buffer.data(), PACKED_PACKET_SIZE)) <= 0) {
        throw std::runtime_error("Disconnect");
    }
#endif
    buffer.resize(n);
    client->addToTcpBuffer(buffer);
    _processTcpBuffer(client);
}

void ServerConnection::_tryReceiveUDP()
{
    std::vector<uint8_t> buffer(PACKED_PACKET_SIZE);

    sockaddr_in addr{};
    std::memset(&addr, 0, sizeof(addr));
    socklen_t len = sizeof(addr);
    int n = 0;
    if ((n = recvfrom(_udpFd, reinterpret_cast<char *>(buffer.data()), sizeof(PacketBuilder::PackedPacket), 0, reinterpret_cast<sockaddr *>(&addr), &len)) <= 0) {
        throw std::runtime_error("Error receiving udp packet");
    }
    buffer.resize(n);
    if (_udpBuffers.find(std::make_pair(inet_ntoa(addr.sin_addr), ntohs(addr.sin_port))) == _udpBuffers.end()) {
        _udpBuffers[std::make_pair(inet_ntoa(addr.sin_addr), ntohs(addr.sin_port))] = buffer;
    } else {
        _udpBuffers[std::make_pair(inet_ntoa(addr.sin_addr), ntohs(addr.sin_port))].insert(_udpBuffers[std::make_pair(inet_ntoa(addr.sin_addr), ntohs(addr.sin_port))].end(), buffer.begin(), buffer.end());
    }
    _processUdpBuffer(std::make_pair(inet_ntoa(addr.sin_addr), ntohs(addr.sin_port)), &addr);
}

void ServerConnection::_processTcpBuffer(Client *client)
{
    if (client->getTcpBuffer().size() >= PACKED_PACKET_SIZE) {
        PacketBuilder::PackedPacket packed = {0};
        std::memcpy(packed, client->getTcpBuffer().data(), sizeof(PacketBuilder::PackedPacket));
        Packet *packet = new Packet;
        try {
            PacketBuilder::unpack(&packed, packet);
        }
        catch (const std::exception &e) {
            delete packet;
            return;
        }
        client->addTcpPacketInput(packet);
        client->getTcpBuffer().erase(client->getTcpBuffer().begin(), client->getTcpBuffer().begin() + sizeof(PacketBuilder::PackedPacket));
    }
}

void ServerConnection::_processUdpBuffer(const std::pair<std::string, int> &addr, sockaddr_in *addr_in)
{
    auto &buffer = _udpBuffers[addr];
    if (buffer.size() >= PACKED_PACKET_SIZE) {
        PacketBuilder::PackedPacket packed = {0};
        std::memcpy(packed, buffer.data(), sizeof(PacketBuilder::PackedPacket));
        auto *packet = new Packet;
        try {
            PacketBuilder::unpack(&packed, packet);
        }
        catch (const std::exception &e) {
            delete packet;
            return;
        }
        if (Client *client = _getClientByID(packet->id); client != nullptr) {
            client->updateUdpAddress(addr_in);
            client->addUdpPacketInput(packet);
        }
        buffer.erase(buffer.begin(), buffer.begin() + sizeof(PacketBuilder::PackedPacket));
    }
}

void ServerConnection::_createSocket()
{
    _udpFd = socket(AF_INET, SOCK_DGRAM, 0);
    _tcpFd = socket(AF_INET, SOCK_STREAM, 0);

    if (_udpFd == -1 || _tcpFd == -1) {
#ifdef WIN32
        if (_udpFd != -1) _close(_udpFd);
        if (_tcpFd != -1) _close(_tcpFd);
#else
        if (_udpFd != -1) close(_udpFd);
        if (_tcpFd != -1) close(_tcpFd);
#endif
        throw std::runtime_error("Error creating sockets");
    }

    std::memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
    _addr.sin_addr.s_addr = INADDR_ANY;
    int optval = 1;
    setsockopt(_tcpFd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&optval), sizeof(optval));
    setsockopt(_udpFd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<const char *>(&optval), sizeof(optval));

    if (_udpFd == -1 || _tcpFd == -1) {

        throw std::runtime_error("Error creating socket");
    }

    if (bind(_tcpFd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) < 0) {
        throw std::runtime_error("Error binding tcp socket");
    }

    listen(_tcpFd, SOMAXCONN);

    if (bind(_udpFd, reinterpret_cast<sockaddr *>(&_addr), sizeof(_addr)) < 0) {
        throw std::runtime_error("Error binding udp socket");
    }
}

void ServerConnection::_setClientFds(fd_set *set)
{
#ifndef far
#define far
#endif
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
#ifdef WIN32
        max = max(max, client->getTcpFd());
#else
        max = std::max(max, client->getTcpFd());
#endif
    }
    return max;
}

int ServerConnection::_selectFd()
{
    int retval;

#ifndef far
#define far
#endif

    timeval timeout = {2, 0};
    _setClientFds(&_readfds);
    _setClientFds(&_writefds);
    FD_SET(_tcpFd, &_readfds);
    FD_SET(_udpFd, &_readfds);
    int maxFd = max(max(_getMaxFd(), _tcpFd), _udpFd);
    retval = select(maxFd + 1, &_readfds, &_writefds, nullptr, &timeout);
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
