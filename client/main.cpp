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
    sprite.position = {1, 100};
    Sprite2D subSprite = Sprite2D("subSprite", "ye2.png");
    subSprite.position = {20, 20};
    rootNode.addChild(sprite);
    sprite.addChild(subSprite);
    client.startGame();
    return 0;
}
