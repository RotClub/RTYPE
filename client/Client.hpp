/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Client
*/

#ifndef CLIENT_HPP_
    #define CLIENT_HPP_

    #include "../common/Engine.hpp"
    #include "connection/ClientConnection.hpp"
    #include "ResourceManager.hpp"

    #include <string>

    class Client {
        public:
            Client(Client const &) = delete;
            void operator=(Client const &) = delete;

            static Client &InitiateInstance(std::string ip, int port);
            static Client &GetInstance();

            std::string getIp() const;
            int getPort() const;
            ClientConnection &getClientConnectionTcp();
            ClientConnection &getClientConnectionUdp();
            ResourceManager &getResourceManager();
        protected:
            Client(std::string ip, int port);
            static Client *_instance;
        private:
            std::string _ip;
            int _port;
            ClientConnection _clientConnectionTcp;
            ClientConnection _clientConnectionUdp;
            ResourceManager _resourceManager;
            // window
            // loadingScreen
    };

#endif /* !CLIENT_HPP_ */
