/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** ServerConnection
*/

#ifndef SERVERCONNECTION_HPP_
    #define SERVERCONNECTION_HPP_

    #include "../common/Networking/GlobalConnection.hpp"
    #include "../common/Networking/Packet.hpp"

    #define KEYWORD "xMmM21B6dFdwJY39"

    class ClientConnection : public GlobalConnection {
    public:
        ClientConnection(const std::string &ip, int port);
        ~ClientConnection();

        void connectToServer();
        void disconnectFromServer();
        void establishConnection();
        void sendToServerTCP(Packet *packet);
        void sendToServerUDP(Packet *packet);
        bool hasPendingTCPPacket();
        bool hasPendingUDPPacket();

        bool isConnected() const { return _connected; }

        void setID(const std::string &id) { std::memcpy(_id, id.data(), sizeof(char[16])); }
        char *getID() { return _id; }

    private:
        void _loop();
        void _receiveLoop();
        void _sendLoop();
        Packet *_tryReceiveTCP();
        Packet *_tryReceiveUDP();

        std::string _ip;
        int _port;
        char _id[16];
        std::atomic<bool> _connected = false;
};

#endif /* !SERVERCONNECTION_HPP_ */
