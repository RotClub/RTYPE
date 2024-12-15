/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** StringPacket
*/

#ifndef STRINGPACKET_HPP_
    #define STRINGPACKET_HPP_

    #include <vector>
    #include <string>

    typedef struct {
        int size;
        std::vector<void *> data;
    } Packet;

    #define NULL_PACKET (Packet){0, {}}

#endif /* !STRINGPACKET_HPP_ */
