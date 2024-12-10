/*
** EPITECH PROJECT, 2024
** Rtype [WSL: Ubuntu]
** File description:
** ServerConnection
*/

#ifndef SERVERCONNECTION_HPP_
    #define SERVERCONNECTION_HPP_

    #include "../common/Networking/GlobalConnection.hpp"
    #include "../common/Networking/Defines.hpp"
    #include "../common/Networking/SafeQueue.hpp"
    #include "../common/Networking/Packet.hpp"
    #include "../common/Networking/PacketBuilder.hpp"
    #include <map>
    #include <thread>
    #include <set>

    #define KEYWORD "xMmM21B6dFdwJY39"

    class ServerConnection : public GlobalConnection {
        public:
            ServerConnection(int port, bool udp = false);
            ~ServerConnection();

            void start();
            void stop();

        private:
            void _loop() override;
            void _receiveLoop();
            void _sendLoop();
            Packet _tryReceive();
            void _createSocket() override;
            int _selectFd() override;


            int _port;
            std::string _ip;
            bool _udp;
            int _max_sd;
            std::set<int> _clientSockets;
            std::atomic<bool> _running = false;
};

#endif /* !SERVERCONNECTION_HPP_ */
