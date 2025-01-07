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
        ClientConnection(std::string ip, int port, bool udp = false);
        ~ClientConnection();

        void connectToServer();
        void disconnectFromServer();
        bool establishConnection();
        void sendToServer(Packet *pckt);
        bool hasPendingPacket();

        bool isConnected() const { return _connected; }

    private:
        void _loop();
        void _receiveLoop();
        void _sendLoop();
        Packet *_tryReceive();

        std::string _ip;
        int _port;
        std::atomic<bool> _connected = false;
};

#endif /* !SERVERCONNECTION_HPP_ */
