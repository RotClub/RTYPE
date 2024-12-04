/*
** EPITECH PROJECT, 2024
** Rtype [WSL: Ubuntu]
** File description:
** ServerConnection
*/

#include "ServerConnection.hpp"

ServerConnection::ServerConnection(int port, bool udp)
    : GlobalConnection(), _port(port), _udp(udp), _running(false)
{
    _serverSocket = socket(AF_INET, _udp ? SOCK_DGRAM : SOCK_STREAM, 0);
    if (_serverSocket == -1)
        throw std::runtime_error("Error creating server socket");

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(port);

    if (bind(_serverSocket, (sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        close(_serverSocket);
        throw std::runtime_error("Error binding server socket");
    }

    if (!_udp && listen(_serverSocket, 10) == -1) {
        close(_serverSocket);
        throw std::runtime_error("Error listening on server socket");
    }
}

ServerConnection::~ServerConnection()
{
    stop();
    close(_serverSocket);
}

void ServerConnection::start()
{
    if (_running) return;
    _running = true;

    if (_udp) {
        _acceptThread = std::thread(&ServerConnection::_clientHandler, this, _serverSocket);
    } else {
        _acceptThread = std::thread(&ServerConnection::_acceptloop, this);
    }
}

void ServerConnection::stop()
{
    if (!_running)
        return;
    _running = false;

    if (_acceptThread.joinable())
        _acceptThread.join();

    if (!_udp) {
        std::lock_guard<std::mutex> lock(_clientMutex);
        for (auto &[clientSocket, thread] : _clientThreads) {
            close(clientSocket);
            if (thread.joinable()) {
                thread.join();
            }
        }
        _clientThreads.clear();
    }
}

bool ServerConnection::isRunning() const
{
    return _running;
}

void ServerConnection::_acceptloop()
{
    while (_running) {
        sockaddr_in clientAddr;
        socklen_t clientAddrSize = sizeof(clientAddr);
        int clientSocket = accept(_serverSocket, (sockaddr *)&clientAddr, &clientAddrSize);
        if (clientSocket == -1) {
            if (_running) {
                throw std::runtime_error("Error accepting client connection");
            }
            continue;
        }

        std::lock_guard<std::mutex> lock(_clientMutex);
        _clientThreads[clientSocket] = std::thread(&ServerConnection::_clientHandler, this, clientSocket);
    }
}

void ServerConnection::_clientHandler(int socket)
{
    while (_running) {
        try {
            Packet packet = _tryReceive(socket);
            if (packet.size == 0) {
                break;
            }

            std::get<IN>(_queues)->enqueue(packet);
        } catch (const std::exception &e) {
            std::cerr << "Handler error: " << e.what() << std::endl;
            break;
        }
    }

    if (!_udp) {
        close(socket);

        std::lock_guard<std::mutex> lock(_clientMutex);
        if (_clientThreads[socket].joinable()) {
            _clientThreads[socket].join();
        }
        _clientThreads.erase(socket);
    }
}

Packet ServerConnection::_tryReceive(int socket)
{
    if (_udp) {
        sockaddr_in clientAddr{};
        socklen_t addrLen = sizeof(clientAddr);
        char buffer[4096];
        ssize_t bytesRead = recvfrom(socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddr, &addrLen);
        if (bytesRead <= 0) {
            throw std::runtime_error("Error receiving UDP packet");
        }

        std::vector<void *> packetData(bytesRead);
        std::memcpy(packetData.data(), buffer, bytesRead);
        return Packet{static_cast<int32_t>(bytesRead), packetData};

    } else {
        int32_t dataSize = 0;
        size_t bytesRead = 0;

        while (bytesRead < sizeof(dataSize)) {
            int result = read(socket, reinterpret_cast<char*>(&dataSize) + bytesRead, sizeof(dataSize) - bytesRead);
            if (result <= 0) {
                throw std::runtime_error("Error reading data size from TCP client");
            }
            bytesRead += result;
        }

        std::vector<void *> buffer(dataSize);
        bytesRead = 0;

        while (bytesRead < dataSize) {
            int result = read(socket, buffer.data() + bytesRead, dataSize - bytesRead);
            if (result <= 0) {
                throw std::runtime_error("Error reading data from TCP client");
            }
            bytesRead += result;
        }

        return Packet{dataSize, buffer};
    }
}

void ServerConnection::_sendPacket(int clientSocket, Packet pckt)
{
    uint32_t dataSize = pckt.size;
    size_t bytesSent = 0;

    while (bytesSent < sizeof(dataSize)) {
        int result = send(clientSocket, reinterpret_cast<char*>(&dataSize) + bytesSent, sizeof(dataSize) - bytesSent, 0);
        if (result == -1) {
            throw std::runtime_error("Error sending data size to client");
        }
        bytesSent += result;
    }

    bytesSent = 0;
    while (bytesSent < dataSize) {
        int result = send(clientSocket, pckt.data.data() + bytesSent, dataSize - bytesSent, 0);
        if (result == -1) {
            throw std::runtime_error("Error sending data to client");
        }
        bytesSent += result;
    }
}

void ServerConnection::broadcast(Packet pckt)
{
    std::lock_guard<std::mutex> lock(_clientMutex);
    for (const auto &[clientSocket, thread] : _clientThreads) {
        try {
            _sendPacket(clientSocket, pckt);
        } catch (const std::exception &e) {
            std::cerr << "Broadcast error: " << e.what() << std::endl;
        }
    }
}
