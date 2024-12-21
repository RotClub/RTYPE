/*
** EPITECH PROJECT, 2024
** Rtype [WSL: Ubuntu]
** File description:
** ServerConnection
*/

#include "ServerConnection.hpp"

#include <Engine.hpp>
#include <cstddef>

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
    _thread = std::thread(&ServerConnection::_loop, this);
}

void ServerConnection::stop()
{
    if (!_running) return;
    _running = false;
    if (_thread.joinable()) {
        _thread.join();
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
            for (auto &client : _clientConnections) {
                if (client->getStep() != Client::ConnectionStep::COMPLETE) {
                    _authFlow(client);
                }
            }
        } catch (const std::exception &e) {
            std::cerr << "[ServerConnection] Error in loop: " << e.what() << std::endl;
        }
    }
}

void ServerConnection::_disconnectClients()
{
    _clientConnections.erase(std::remove_if(_clientConnections.begin(), _clientConnections.end(), [](Client *client) {
        if (client->shouldDisconnect()) {
            spdlog::debug("Client (fd {}) disconnected", client->getTcpFd());
            delete client;
            return true;
        }
        return false;
    }), _clientConnections.end());
}

void ServerConnection::_receiveLoop()
{
    for (auto &client : _clientConnections) {
        if (!client->shouldDisconnect() && FD_ISSET(client->getTcpFd(), &_readfds)) {
            try {
                Packet packet = _tryReceiveTCP(client);
                client->addTcpPacketInput(packet);
                spdlog::info("Packet received from client");
            } catch (const std::exception &e) {
                client->disconnect();
            }
        }
    }
}

void ServerConnection::_authFlow(Client *client)
{
    // PacketBuilder builder;
    // Packet *packet = builder.writeString(Engine::GetInstance().getGameInfo()->getName()).build();
    //
    // client->addTcpPacketOutput(packet);
    // client->setStep(Client::ConnectionStep::COMPLETE);
}

void ServerConnection::_sendLoop()
{
    for (auto &client : _clientConnections) {
        if (!client->shouldDisconnect() && FD_ISSET(client->getTcpFd(), &_writefds) && client->hasTcpPacketOutput()) {
            while (client->hasTcpPacketOutput()) {
                Packet packet = client->popTcpPacketOutput();
                size_t len = packet.n;
                write(client->getTcpFd(), &packet, len);
                std::free(packet.data);
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

Packet ServerConnection::_tryReceiveTCP(Client *client)
{
    Packet packet = NULL_PACKET;
    int dataSize = 0;

    if (read(client->getTcpFd(), &dataSize, sizeof(size_t)) <= 0) {
        throw std::runtime_error("Disconnect");
    }
    packet.n = dataSize;
    packet.cmd = PacketCmd::NONE;
    packet.data = malloc(dataSize);
    if (read(client->getTcpFd(), &packet, sizeof(PacketCmd) + dataSize) <= 0) {
        throw std::runtime_error("Disconnect");
    }
    return packet;
}

void ServerConnection::_createSocket()
{
    _udpFd = socket(AF_INET, SOCK_DGRAM, 0);
    _tcpFd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&_addr, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(_port);
    _addr.sin_addr.s_addr = INADDR_ANY;

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
