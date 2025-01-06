/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Ubuntu-20.04]
** File description:
** PacketBuilder
*/

#include "PacketBuilder.hpp"
#include "Networking/Packet.hpp"
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <string>

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

PacketBuilder PacketBuilder::setCmd(PacketCmd cmd)
{
    _cmd = cmd;
    return *this;
}

PacketBuilder &PacketBuilder::writeInt(int nb)
{
    if (_data == nullptr)
        _data = new char[0];
    _n += sizeof(int);
    void *rt = realloc(_data, _n);
    if (rt == NULL)
        throw std::runtime_error("Error reallocating memory");
    _data = rt;
    std::memcpy(static_cast<char*>(_data) + _n - sizeof(int), &nb, sizeof(int));
    return *this;
}

PacketBuilder &PacketBuilder::writeString(const std::string &str)
{
    if (_data == nullptr)
        _data = new char[0];
    const char *cstr = str.c_str();
    _n += sizeof(char) * str.length() + 1;
    void *rt = realloc(_data, _n);
    if (rt == NULL)
        throw std::runtime_error("Error reallocating memory");
    _data = rt;
    size_t len = sizeof(char) * str.length() + 1;
    std::memcpy(static_cast<char*>(_data) + _n - len, cstr, len);
    return *this;
}

int PacketBuilder::readInt()
{
    int nb = 0;
    std::memcpy(&nb, _data, sizeof(int));
    _data = static_cast<char*>(_data) + sizeof(int);
    _n -= sizeof(int);
    return nb;
}

std::string PacketBuilder::readString()
{
    std::string str(static_cast<char*>(_data));
    _data = static_cast<char*>(_data) + sizeof(char) * str.length() + 1;
    _n -= sizeof(char) * str.length() + 1;
    return str;
}

Packet *PacketBuilder::build()
{
    Packet *packet = new Packet;
    packet->n = _n;
    packet->cmd = _cmd;
    packet->data = _data;
    _n = 0;
    _cmd = PacketCmd::NONE;
    _data = nullptr;
    return packet;
}
