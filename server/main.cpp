/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Server.hpp"

int main(int argc, char **argv)
{
    try {
        Server srv(25777);
        srv.start();
    } catch (const std::exception &e) {
        std::cerr << "Could not start server: " << e.what() << std::endl;
        return 84;
    }
    return 0;
}
