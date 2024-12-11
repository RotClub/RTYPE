#include "PacketBuilder.hpp"

PacketBuilder::PacketBuilder()
{
    _packet = new Packet;
    _packet->size = NULL_PACKET.size;
    _packet->data = NULL_PACKET.data;
}

PacketBuilder::PacketBuilder(Packet packet)
{
    _packet = new Packet;
    _packet->data = packet.data;
    _packet->size = packet.size;
}

PacketBuilder &PacketBuilder::writeInt(int nb)
{
    int &data = nb;
    void *ptr = &data; // for testing
    _packet->data.push_back(ptr);
    _packet->size += sizeof(int);
    return *this;
}

PacketBuilder &PacketBuilder::writeString(std::string str)
{
    std::vector<char> data(str.begin(), str.end());
    void *ptr = data.data();
    _packet->data.push_back(ptr);
    _packet->size += data.size();
    return *this;
}

int PacketBuilder::readInt()
{
    return *(int *)_popFront();
}

std::string PacketBuilder::readString()
{
    std::vector<char> data((char *)_popFront(), (char *)_popFront() + _packet->size);
    return std::string(data.begin(), data.end());
}

Packet *PacketBuilder::build()
{
    return _packet;
}

void *PacketBuilder::_popFront()
{
    void *ptr = _packet->data.front();
    _packet->data.erase(_packet->data.begin());
    return ptr;
}
