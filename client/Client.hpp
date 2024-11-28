/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Client
*/

#ifndef CLIENT_HPP_
    #define CLIENT_HPP_

    #include <string>
    #include "../common/Engine.hpp"

    class Client {
        public:
            Client(Client const &) = delete;
            void operator=(Client const &) = delete;

            static Client *InitiateInstance(std::string ip, int port);
            static Client *GetInstance();

            std::string getIp() const;
            int getPort() const;
        protected:
            Client(std::string ip, int port);
            static Client *_instance;
        private:
            std::string _ip;
            int _port;
            // socket
            // serverDataManager
            // window
            // loadingScreen
            // resourceManager
    };

#endif /* !CLIENT_HPP_ */
