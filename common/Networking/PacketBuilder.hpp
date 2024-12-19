/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** PacketUtils
*/

#ifndef PACKETUTILS_HPP_
    #define PACKETUTILS_HPP_

    #include "Packet.hpp"

    #include <array>
    #include <queue>
    #include <string>
    #include <vector>

    class PacketBuilder {
        public:
            PacketBuilder();
            PacketBuilder(Packet *packet);
            ~PacketBuilder() = default;

            PacketBuilder setCmd(PacketCmd cmd);

            PacketBuilder &writeInt(int nb);
            PacketBuilder &writeString(const std::string str);

            int readInt();
            std::string readString();

            Packet *build();

            void destroyPacket();

        private:
            Packet _packet;

    };

#endif /* !PACKETUTILS_HPP_ */
