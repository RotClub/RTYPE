//
// Created by Théo Bary on 10/12/2024.
//

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <Networking/Packet.hpp>
#include <Networking/SafeQueue.hpp>
#include <cstring>

#ifdef WIN32
    #include <WindowsCross.hpp>
#else
    #include <netinet/in.h>
#endif

#include <string>
#include <tuple>
#include <cstdint>
#include <Networking/Defines.hpp>

class Client
{
    public:
        Client(int srvTcpFd);
        ~Client();

        enum class ConnectionStep
        {
            UNVERIFIED,
            AUTH_CODE_SENT,
            AUTH_CODE_VERIFIED,
            COMPLETE
        };

        [[nodiscard]] int getTcpFd() const { return _tcpFd; }
        [[nodiscard]] sockaddr_in *getAddress() { return &_address; }
        [[nodiscard]] sockaddr_in *getUdpAddress() { return &_udpAddress; }
        [[nodiscard]] ConnectionStep getStep() const { return _step; }
        [[nodiscard]] const std::string &getUuid() const { return uuid; }

        void setStep(const ConnectionStep step) { _step = step; }

        void updateUdpAddress(sockaddr_in *addr) { std::memcpy(&_udpAddress, addr, sizeof(sockaddr_in)); }

        void addTcpPacketInput(Packet *packet);
        void addUdpPacketInput(Packet *packet);
        Packet *popTcpPacketInput();
        Packet *popUdpPacketInput();
        void addTcpPacketOutput(Packet *packet);
        void addUdpPacketOutput(Packet *packet);
        Packet *popTcpPacketOutput();
        Packet *popUdpPacketOutput();
        bool hasTcpPacketOutput();
        bool hasUdpPacketOutput();
        bool hasTcpPacketInput();
        bool hasUdpPacketInput();

        // TODO REMOVE
        int getUdpQueueSize() { return std::get<PACKET_OUT>(_udpQueues).size(); }

        void addToTcpBuffer(const std::vector<uint8_t> &buffer) { _tcpBuffer.insert(_tcpBuffer.end(), buffer.begin(), buffer.end()); }
        std::vector<uint8_t> &getTcpBuffer() { return _tcpBuffer; }

        void disconnect() { _shouldDisconnect = true; }
        [[nodiscard]] bool shouldDisconnect() const { return _shouldDisconnect; }

        char *getID() { return _id; }

    private:
        const std::string uuid;
        int _tcpFd;
        sockaddr_in _address;
        sockaddr_in _udpAddress;
        ConnectionStep _step;
        std::tuple<SafeQueue<Packet *>, SafeQueue<Packet *>> _tcpQueues;
        std::tuple<SafeQueue<Packet *>, SafeQueue<Packet *>> _udpQueues;
        bool _shouldDisconnect;
        char _id[16];
        std::vector<uint8_t> _tcpBuffer;
};

#endif // CLIENT_HPP
