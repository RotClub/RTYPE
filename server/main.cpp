/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Server.hpp"

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
    Server srv(25777);
    signal(SIGINT, reinterpret_cast<void (*)(int)>(sigHandler));
    signal(SIGTERM, reinterpret_cast<void (*)(int)>(sigHandler));
    sigHandler(-1, &srv);
    srv.start();
    return 0;
}
