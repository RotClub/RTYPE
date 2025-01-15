/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** PacketUtils
*/

#ifndef PACKETUTILS_HPP_
#define PACKETUTILS_HPP_

#define PACKED_PACKET_SIZE 4096

#include "Packet.hpp"

#include <cstddef>
#include <string>

class PacketBuilder
{
    public:
        static const std::string integrityChallenge;
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
        char _id[16];
        void *_data;
};

#endif /* !PACKETUTILS_HPP_ */
