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
    Sprite2D sprite = Sprite2D("sprite", "assets/a.png");
    sprite.position = {30, 100};
    Sprite2D subSprite = Sprite2D("subSprite", "assets/b.png");
    subSprite.position = {350, 50};
    rootNode.addChild(sprite);
    sprite.addChild(subSprite);
    client.startGame();
    return 0;
}
