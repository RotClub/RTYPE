/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** PacketUtils
*/

#ifndef PACKETUTILS_HPP_
#define PACKETUTILS_HPP_

#define PACKED_PACKET_SIZE 4096
#define PACKED_PACKET_INTEGRITY_CHALLENGE "Y4DrMrLiwlli79jzU9v8AHLH1IaNyBo4"

#include "Packet.hpp"

#include <cstddef>
#include <string>

class PacketBuilder
{
    public:
        PacketBuilder();
        PacketBuilder(Packet *packet);
        ~PacketBuilder() = default;

        void loadFromPacket(Packet *packet);

        PacketBuilder &setCmd(PacketCmd cmd);

        PacketBuilder &writeInt(int nb);
        PacketBuilder &writeFloat(float nb);
        PacketBuilder &writeString(const std::string &str);

        [[nodiscard]] int readInt();
        [[nodiscard]] float readFloat();
        [[nodiscard]] std::string readString();

        Packet *build();
        void reset();

        using PackedPacket = char[PACKED_PACKET_SIZE];

        static void pack(PackedPacket *packed, const Packet *packet);
        static void unpack(const PackedPacket *packed, Packet *packet);

    private:
        size_t _n;
        PacketCmd _cmd;
        void *_data;
};

#endif /* !PACKETUTILS_HPP_ */
