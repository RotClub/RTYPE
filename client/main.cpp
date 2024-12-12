/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Client.hpp"
#include "raylib-cpp.hpp"

#include "Nodes/Node2D/Sprite2D/Sprite2D.hpp"

int main(void)
{
    Engine &engine = Engine::StartInstance(Types::VMState::CLIENT, "rtype");
    Client &client = Client::InitiateInstance("127.0.0.1", 25777);
    engine.Log(Engine::LogLevel::INFO, "Client started");
    Node rootNode = Node("root");
    Engine::GetInstance().root = &rootNode;
    Sprite2D sprite = Sprite2D("sprite", "ye.png");
    sprite.position = {100, 100};
    rootNode.AddChild(sprite);
    client.startGame();
    return 0;
}
