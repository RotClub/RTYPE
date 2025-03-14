/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** PacketBuilder
*/

#include "PacketBuilder.hpp"
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>
#include "Networking/Packet.hpp"

#include "spdlog/spdlog.h"

const std::string PacketBuilder::integrityChallenge = "Y4DrMrLiwlli79jzU9v8AHLH1IaNyBo4";

PacketBuilder::PacketBuilder()
{
    _n = 0;
    _cmd = PacketCmd::NONE;
    _data = nullptr;
    std::memset(_id, 0, sizeof(_id));
}

PacketBuilder::PacketBuilder(Packet *packet)
{
    _n = packet->n;
    _cmd = packet->cmd;
    _data = packet->data;
    std::memcpy(_id, packet->id, sizeof(packet->id));
}

void PacketBuilder::loadFromPacket(Packet *packet)
{
    _cmd = packet->cmd;
    _data = packet->data;
    _n = packet->n;
    std::memcpy(_id, packet->id, sizeof(packet->id));
}

PacketBuilder &PacketBuilder::setCmd(PacketCmd cmd)
{
    _cmd = cmd;
    return *this;
}

PacketBuilder &PacketBuilder::writeInt(const int nb)
{
    void *rt = std::realloc(_data, _n + sizeof(int));
    if (rt == nullptr)
        throw std::runtime_error("Error reallocating memory");
    _data = rt;
    std::memcpy(static_cast<char *>(_data) + _n, &nb, sizeof(int));
    _n += sizeof(int);
    return *this;
}

PacketBuilder &PacketBuilder::writeFloat(const float nb)
{
    void *rt = std::realloc(_data, _n + sizeof(float));
    if (rt == nullptr)
        throw std::runtime_error("Error reallocating memory");
    _data = rt;
    std::memcpy(static_cast<char *>(_data) + _n, &nb, sizeof(float));
    _n += sizeof(float);
    return *this;
}

PacketBuilder &PacketBuilder::writeBool(const bool nb)
{
    void *rt = std::realloc(_data, _n + sizeof(bool));
    if (rt == nullptr)
        throw std::runtime_error("Error reallocating memory");
    _data = rt;
    std::memcpy(static_cast<char *>(_data) + _n, &nb, sizeof(bool));
    _n += sizeof(bool);
    return *this;
}

PacketBuilder &PacketBuilder::writeString(const std::string &str)
{
    size_t len = str.length() + 1;
    void *rt = std::realloc(_data, _n + len);
    if (rt == nullptr)
        throw std::runtime_error("Error reallocating memory");
    _data = rt;
    std::memcpy(static_cast<char *>(_data) + _n, str.c_str(), len);
    _n += len;
    return *this;
}

int PacketBuilder::readInt()
{
    if (_n < sizeof(int))
        throw std::runtime_error("Not enough data to read an int");
    int nb = 0;
    std::memcpy(&nb, _data, sizeof(int));
    _data = static_cast<char *>(_data) + sizeof(int);
    _n -= sizeof(int);
    return nb;
}

float PacketBuilder::readFloat()
{
    if (_n < sizeof(float))
        throw std::runtime_error("Not enough data to read a float");
    float nb = 0;
    std::memcpy(&nb, _data, sizeof(float));
    _data = static_cast<char *>(_data) + sizeof(float);
    _n -= sizeof(float);
    return nb;
}

bool PacketBuilder::readBool()
{
    if (_n < sizeof(bool))
        throw std::runtime_error("Not enough data to read a bool");
    bool nb = false;
    std::memcpy(&nb, _data, sizeof(bool));
    _data = static_cast<char *>(_data) + sizeof(bool);
    _n -= sizeof(bool);
    return nb;
}

std::string PacketBuilder::readString()
{
    size_t len = std::strlen(static_cast<char *>(_data)) + 1;
    if (_n < len)
        throw std::runtime_error("Not enough data to read a string");
    std::string str(static_cast<char *>(_data));
    _data = static_cast<char *>(_data) + len;
    _n -= len;
    return str;
}

Packet *PacketBuilder::build()
{
    Packet *packet = new Packet;
    packet->n = _n;
    packet->cmd = _cmd;
    packet->data = _data;
    std::memcpy(packet->id, _id, sizeof(_id));
    _n = 0;
    _cmd = PacketCmd::NONE;
    _data = nullptr;
    std::memset(_id, 0, sizeof(_id));
    return packet;
}

void PacketBuilder::reset()
{
    if (_n > 0 && _data != nullptr)
        std::free(_data);
    _n = 0;
    _cmd = PacketCmd::NONE;
    _data = nullptr;
    std::memset(_id, 0, sizeof(_id));
}

void PacketBuilder::copy(Packet *dest, const Packet *src)
{
    std::memcpy(dest, src, sizeof(Packet));
    if (src->data == nullptr || src->n == 0)
        return;
    dest->data = std::malloc(src->n);
    std::memcpy(dest->data, src->data, src->n);
}

void PacketBuilder::pack(PackedPacket *packed, const Packet *packet)
{
    std::memset(*packed, 0, sizeof(PackedPacket));
    std::memcpy(*packed, integrityChallenge.c_str(), sizeof(char) * 32);

    size_t offset = 32;
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

void PacketBuilder::unpack(const PackedPacket *packed, Packet *packet)
{
    size_t offset = 32;

    if (std::memcmp(*packed, integrityChallenge.c_str(), sizeof(char) * 32) != 0) {
        throw std::runtime_error("Invalid packet integrity challenge.");
    }

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
        try {
            std::memcpy(packet->data, *packed + offset, packet->n);
        } catch (...) {
            std::free(packet->data);
            throw;
        }
        offset += packet->n;
    }
    else {
        packet->data = nullptr;
    }
}
