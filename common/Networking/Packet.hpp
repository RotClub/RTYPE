/*
** EPITECH PROJECT, 2024
** RTYPE [WSL: Arch]
** File description:
** StringPacket
*/

#ifndef STRINGPACKET_HPP_
    #define STRINGPACKET_HPP_

    #include <any>
    #include <cstddef>
    #include <vector>

    enum class PacketCmd {
        NONE,
        CONNECT,
        DISCONNECT,
        NET,
        NEW_MESSAGE
    };

    using Packet = struct packet_s {
        size_t n;
        PacketCmd cmd;
        void *data;
    };

#endif /* !STRINGPACKET_HPP_ */
