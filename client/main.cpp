/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Client.hpp"
#include "Networking/PacketBuilder.hpp"
#include "raylib-cpp.hpp"

#include "Nodes/Node2D/Sprite2D/Sprite2D.hpp"
#include "spdlog/spdlog.h"
#include <fstream>
#include <ostream>

int main(void)
{
    Engine &engine = Engine::StartInstance(Types::VMState::CLIENT, "rtype");
    Client &client = Client::InitiateInstance("127.0.0.1", 25777);
    try {
        client.getClientConnectionTcp().connectToServer();
    } catch (const std::exception &e) {
        spdlog::error(e.what());
        return 84;
    }

    PacketBuilder toBuilder;
    Packet *toPacket = toBuilder
        .writeInt(7654)
        .writeInt(43)
        .writeString("Hey!")
        .writeInt(123)
        .writeString("Hello!")
        .writeString("This is a test, this is only a test.")
        .writeInt(456)
        .writeString("Goodbye! See you later! :D")
        .build();

    client.getClientConnectionTcp().sendToServer(toPacket);

    bool stop = false;
    while (!stop) {
        Packet *packet = client.getClientConnectionTcp().getLatestPacket();
        if (packet) {
            PacketBuilder fromBuilder(packet);
            int a = fromBuilder.readInt();
            std::string b = fromBuilder.readString();
            spdlog::debug("Received: {} {}", a, b);
            stop = true;
        }
    }
}
