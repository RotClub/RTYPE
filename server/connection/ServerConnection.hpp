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

    #define KEYWORD "xMmM21B6dFdwJY39"

    class ServerConnection : public GlobalConnection {
    public:
        ServerConnection(int port, bool udp = false);
        ~ServerConnection();

        void start();
        void stop();
        bool isRunning() const;
        void broadcast(Packet pckt);
        void handleClients();

    private:
        void _acceptloop();
        void _clientHandler(int clientSocket);
        Packet _tryReceive(int clientSocket);
        void _sendPacket(int clientSocket, Packet pckt);

        int _port;
        bool _udp;
        int _serverSocket;
        std::atomic<bool> _running = false;
        std::thread _acceptThread;
        std::map<int, std::thread> _clientThreads;
        std::mutex _clientMutex;
};

#endif /* !SERVERCONNECTION_HPP_ */
