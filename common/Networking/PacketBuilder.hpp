/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** PacketUtils
*/

#ifndef PACKETUTILS_HPP_
    #define PACKETUTILS_HPP_

    #include "Packet.hpp"

    #include <cstddef>
    #include <string>

    class PacketBuilder {
        public:
            PacketBuilder();
            PacketBuilder(Packet *packet);
            ~PacketBuilder() = default;

            void loadFromPacket(Packet *packet);

            PacketBuilder setCmd(PacketCmd cmd);

            PacketBuilder &writeInt(int nb);
            PacketBuilder &writeString(const std::string &str);

            int readInt();
            std::string readString();

            Packet *build();
            void destroyPacket();

        private:
            size_t _n;
            PacketCmd _cmd;
            void *_data;

    };

#endif /* !PACKETUTILS_HPP_ */
