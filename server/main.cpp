/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Server.hpp"
#include "ArgumentManager/ArgumentManager.hpp"

static void sigHandler(const int sig, Server *srv)
{
    static Server *saved = nullptr;

    if (saved == nullptr)
        saved = srv;
    if (sig != SIGINT && sig != SIGTERM)
        return;
    saved->stop();
}

int main(int argc, char **argv)
{
    try {
        ArgumentManager manager(argc, argv);
        manager.addRequiredArgument("game");
        manager.addDefaultArgument("port", "25777");
        manager.parseArguments();
        if (!manager.checkServerArguments())
            ArgumentManager::DisplayServerUsage();

        Server srv(manager.getArgument("game"), std::stoi(manager.getArgument("port")));

        signal(SIGINT, reinterpret_cast<void (*)(int)>(sigHandler));
        signal(SIGTERM, reinterpret_cast<void (*)(int)>(sigHandler));
        sigHandler(-1, &srv);

        srv.start();
    }
    catch (const std::exception &e) {
        std::cerr << "Could not start server: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
