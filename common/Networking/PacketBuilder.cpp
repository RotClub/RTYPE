#include "PacketBuilder.hpp"

PacketBuilder::PacketBuilder()
{
    _packet = NULL_PACKET;
}

PacketBuilder::PacketBuilder(Packet pckt)
{
    _packet = pckt;
}

PacketBuilder &PacketBuilder::writeInt(int nb)
{
    int &data = nb;
    void *ptr = &data; // for testing
    _packet.data.push_back(ptr);
    _packet.size += sizeof(int);
    return *this;
}

PacketBuilder &PacketBuilder::writeString(std::string str)
{
    std::vector<char> data(str.begin(), str.end());
    void *ptr = data.data();
    _packet.data.push_back(ptr);
    _packet.size += data.size();
    return *this;
}

int PacketBuilder::readInt()
{
    return *(int *)_popFront();
}

std::string PacketBuilder::readString()
{
    std::vector<char> data((char *)_popFront(), (char *)_popFront() + _packet.size);
    return std::string(data.begin(), data.end());
}

Packet PacketBuilder::build()
{
    return _packet;
}

void *PacketBuilder::_popFront()
{
    void *ptr = _packet.data.front();
    _packet.data.erase(_packet.data.begin());
    return ptr;
}
