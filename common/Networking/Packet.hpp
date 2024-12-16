/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** StringPacket
*/

#ifndef STRINGPACKET_HPP_
    #define STRINGPACKET_HPP_

    #include <any>
    #include <vector>

    using Packet = struct packet_s {
        int size;
        std::vector<std::any> data;
    };

    #define NULL_PACKET (Packet){0, {}}

#endif /* !STRINGPACKET_HPP_ */
