/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** PacketBuilder
*/

#include "PacketBuilder.hpp"
#include "Networking/Packet.hpp"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

#include "spdlog/spdlog.h"

PacketBuilder::PacketBuilder()
{
    _n = 0;
    _cmd = PacketCmd::NONE;
    _data = nullptr;
}

PacketBuilder::PacketBuilder(Packet *packet)
{
    _n = packet->n;
    _cmd = packet->cmd;
    _data = packet->data;
}

void PacketBuilder::loadFromPacket(Packet* packet)
{
    _cmd = packet->cmd;
    _data = packet->data;
    _n = packet->n;
}

PacketBuilder &PacketBuilder::setCmd(PacketCmd cmd)
{
    _cmd = cmd;
    return *this;
}

PacketBuilder &PacketBuilder::writeInt(int nb)
{
    void *rt = std::realloc(_data, _n + sizeof(int));
    if (rt == nullptr)
        throw std::runtime_error("Error reallocating memory");
    _data = rt;
    std::memcpy(static_cast<char*>(_data) + _n, &nb, sizeof(int));
    _n += sizeof(int);
    return *this;
}

PacketBuilder &PacketBuilder::writeString(const std::string &str)
{
    size_t len = str.length() + 1;
    void *rt = std::realloc(_data, _n + len);
    if (rt == nullptr)
        throw std::runtime_error("Error reallocating memory");
    _data = rt;
    std::memcpy(static_cast<char*>(_data) + _n, str.c_str(), len);
    _n += len;
    return *this;
}

int PacketBuilder::readInt()
{
    if (_n < sizeof(int))
        throw std::runtime_error("Not enough data to read an int");
    int nb = 0;
    std::memcpy(&nb, _data, sizeof(int));
    _data = static_cast<char*>(_data) + sizeof(int);
    _n -= sizeof(int);
    return nb;
}

std::string PacketBuilder::readString()
{
    size_t len = std::strlen(static_cast<char*>(_data)) + 1;
    if (_n < len)
        throw std::runtime_error("Not enough data to read a string");
    std::string str(static_cast<char*>(_data));
    _data = static_cast<char*>(_data) + len;
    _n -= len;
    return str;
}

Packet *PacketBuilder::build()
{
    Packet *packet = new Packet;
    packet->n = _n;
    packet->cmd = _cmd;
    packet->data = _data;
    std::memset(packet->id, 0, sizeof(packet->id));
    _n = 0;
    _cmd = PacketCmd::NONE;
    _data = nullptr;
    return packet;
}

void PacketBuilder::reset()
{
    if (_n > 0 && _data != nullptr)
        std::free(_data);
    _n = 0;
    _cmd = PacketCmd::NONE;
    _data = nullptr;
}

void PacketBuilder::pack(PackedPacket* packed, const Packet* packet)
{
    std::memset(*packed, 0, PACKED_PACKET_SIZE);

    size_t offset = 0;
    std::memcpy(*packed + offset, &packet->n, sizeof(packet->n));
    offset += sizeof(packet->n);

    std::memcpy(*packed + offset, &packet->cmd, sizeof(packet->cmd));
    offset += sizeof(packet->cmd);

    std::memcpy(*packed + offset, packet->id, sizeof(packet->id));
    offset += sizeof(packet->id);

    if (packet->data != nullptr && packet->n > 0) {
        std::memcpy(*packed + offset, packet->data, packet->n);
        offset += packet->n;
    }
}

void PacketBuilder::unpack(const PackedPacket* packed, Packet* packet)
{
    size_t offset = 0;

    std::memcpy(&packet->n, *packed + offset, sizeof(packet->n));
    offset += sizeof(packet->n);

    std::memcpy(&packet->cmd, *packed + offset, sizeof(packet->cmd));
    offset += sizeof(packet->cmd);

    std::memcpy(packet->id, *packed + offset, sizeof(packet->id));
    offset += sizeof(packet->id);

    if (packet->n > 0) {
        packet->data = malloc(packet->n);
        if (!packet->data) {
            throw std::runtime_error("Failed to allocate memory for packet data.");
        }
        std::memcpy(packet->data, *packed + offset, packet->n);
        offset += packet->n;
    } else {
        packet->data = nullptr;
    }
}
