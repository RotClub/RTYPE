/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** PacketUtils
*/

#ifndef PACKETUTILS_HPP_
    #define PACKETUTILS_HPP_

    #include "Packet.hpp"

    #include <string>

    class PacketBuilder {
        public:
            PacketBuilder();
            PacketBuilder(Packet packet);
            ~PacketBuilder() = default;

            PacketBuilder &writeInt(int nb);
            PacketBuilder &writeString(std::string str);

            int readInt();
            std::string readString();

            Packet *build();

        private:
            Packet *_packet;

            void *_popFront();
    };

#endif /* !PACKETUTILS_HPP_ */
