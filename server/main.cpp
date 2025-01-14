/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Server.hpp"
#include "argumentManager/ArgumentManager.hpp"

static void sigHandler(const int sig, Server* srv)
{
    static Server* saved = nullptr;

    if (saved == nullptr)
        saved = srv;
    if (sig != SIGINT && sig != SIGTERM)
        return;
    saved->stop();
}

int main(int argc, char** argv)
{
    try {
        ArgumentManager argManager(argc, argv);
        if (!argManager.checkArguments())
            return 84;
        Server srv(argc == 1 ? 25777 : std::stoi(argv[2]));
        signal(SIGINT, reinterpret_cast<void (*)(int)>(sigHandler));
        signal(SIGTERM, reinterpret_cast<void (*)(int)>(sigHandler));
        sigHandler(-1, &srv);

        srv.start();
    }
    catch (const std::exception& e) {
        std::cerr << "Could not start server: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
