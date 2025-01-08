/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** main
*/

#include "Server.hpp"

int main(int argc, char **argv)
{
    Server srv(25777);
    srv.start();
    return 0;
}
