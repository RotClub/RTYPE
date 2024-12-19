/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** PacketBuilder
*/

#include "PacketBuilder.hpp"
#include "Networking/Packet.hpp"
#include <any>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include <vector>

PacketBuilder::PacketBuilder()
{
    _packet = NULL_PACKET;
}

PacketBuilder::PacketBuilder(Packet *packet)
{
    _packet = *packet;
}

PacketBuilder PacketBuilder::setCmd(PacketCmd cmd)
{
    _packet.cmd = cmd;
    return *this;
}

PacketBuilder &PacketBuilder::writeInt(int nb)
{
    _packet.n += sizeof(int);
    void *rt = realloc(_packet.data, _packet.n);
    if (rt == NULL)
        throw std::runtime_error("Error reallocating memory");
    _packet.data = rt;
    std::memcpy(static_cast<char*>(_packet.data) + _packet.n - sizeof(int), &nb, sizeof(int));
    return *this;
}

PacketBuilder &PacketBuilder::writeString(const std::string &str)
{
    const char *cstr = str.c_str();
    _packet.n += sizeof(char) * str.length() + 1;
    void *rt = realloc(_packet.data, _packet.n);
    if (rt == NULL)
        throw std::runtime_error("Error reallocating memory");
    _packet.data = rt;
    size_t len = sizeof(char) * str.length() + 1;
    std::memcpy(static_cast<char*>(_packet.data) + _packet.n - len, cstr, len);
    return *this;
}

int PacketBuilder::readInt()
{
    int nb = 0;
    std::memcpy(&nb, _packet.data, sizeof(int));
    _packet.data = static_cast<char*>(_packet.data) + sizeof(int);
    _packet.n -= sizeof(int);
    return nb;
}

std::string PacketBuilder::readString()
{
    std::string str(static_cast<char*>(_packet.data));
    _packet.data = static_cast<char*>(_packet.data) + sizeof(char) * str.length() + 1;
    _packet.n -= sizeof(char) * str.length() + 1;
    return str;
}

Packet *PacketBuilder::build()
{
    return &_packet;
}

void PacketBuilder::destroyPacket()
{
    free(_packet.data);
    _packet = NULL_PACKET;
}
