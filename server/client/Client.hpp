//
// Created by Théo Bary on 10/12/2024.
//

#ifndef CLIENT_HPP
    #define CLIENT_HPP

    #include <netinet/in.h>

class Client {
    public:
        Client(int fd, sockaddr_in address);
        ~Client();

        [[nodiscard]] int getTcpFd() const { return _tcpFd; }
        [[nodiscard]] sockaddr_in getAddress() const { return _address; }

    private:
        int _tcpFd;
        sockaddr_in _address;
};

#endif //CLIENT_HPP
