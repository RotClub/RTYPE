//
// Created by Théo Bary on 10/12/2024.
//

#ifndef CLIENT_HPP
    #define CLIENT_HPP

    #include <tuple>
    #include <netinet/in.h>
    #include <Networking/Packet.hpp>
    #include <Networking/SafeQueue.hpp>

class Client {
    public:
        Client(int srvTcpFd);
        ~Client();

        enum class ConnectionStep {
            UNVERIFIED,
            AUTH_CODE_SENT,
            AUTH_CODE_VERIFIED,
            COMPLETE
        };

        [[nodiscard]] int getTcpFd() const { return _tcpFd; }
        [[nodiscard]] sockaddr_in getAddress() const { return _address; }
        [[nodiscard]] ConnectionStep getStep() const { return _step; }

        void setStep(const ConnectionStep step) { _step = step; }

        void addTcpPacketInput(Packet packet);
        Packet popTcpPacketInput();
        void addTcpPacketOutput(Packet packet);
        Packet popTcpPacketOutput();
        bool hasTcpPacketOutput();

        void disconnect() { _shouldDisconnect = true; }
        [[nodiscard]] bool shouldDisconnect() const { return _shouldDisconnect; }

    private:
        int _tcpFd;
        sockaddr_in _address;
        ConnectionStep _step;
        std::tuple<SafeQueue<Packet>, SafeQueue<Packet>> _tcpQueues;
        std::tuple<SafeQueue<Packet>, SafeQueue<Packet>> _udpQueues;
        bool _shouldDisconnect;
};

#endif //CLIENT_HPP
