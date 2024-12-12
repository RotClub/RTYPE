/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Client.hpp"
#include "raylib-cpp.hpp"

int main(void)
{
    Engine &engine = Engine::StartInstance(Types::VMState::CLIENT, "rtype");
    Client &client = Client::InitiateInstance("127.0.0.1", 25777);
    engine.Log(Engine::LogLevel::INFO, "Client started");
    try {
    client.getClientConnectionTcp().connectToServer();
    }
    catch(const std::exception& e) {
        engine.Log(Engine::LogLevel::ERROR, e.what());
        return 1;
    }
    PacketBuilder builder;
    Packet *packet = builder.writeString("Hello").build();
    client.getClientConnectionTcp().sendToServer(packet);
    while (true) {
        Packet *pckt = client.getClientConnectionTcp().getLatestPacket();
        if (pckt != nullptr) {
            engine.Log(Engine::LogLevel::INFO, "Received packet from server");
            PacketBuilder builder(pckt);
            std::string rcvStr = builder.readString();
            engine.Log(Engine::LogLevel::INFO, "Received: " + rcvStr);
        }
    }
    return 0;
}
