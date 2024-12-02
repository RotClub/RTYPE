/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Client
*/

#ifndef CLIENT_HPP_
    #define CLIENT_HPP_

    #include "../common/Engine.hpp"
    #include "connection/ServerConnection.hpp"

    #include <string>

    class Client {
        public:
            Client(Client const &) = delete;
            void operator=(Client const &) = delete;

            static Client &InitiateInstance(std::string ip, int port);
            static Client &GetInstance();

            std::string getIp() const;
            int getPort() const;
            ClientConnection &getServerConnection();
        protected:
            Client(std::string ip, int port);
            static Client *_instance;
        private:
            std::string _ip;
            int _port;
            ClientConnection _serverConnection;
            // window
            // loadingScreen
            // resourceManager
    };

#endif /* !CLIENT_HPP_ */
