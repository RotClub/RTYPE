/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Types
*/

#ifndef TYPES_HPP_
    #define TYPES_HPP_

namespace Types {
    class Vector2 {
        public:
            Vector2(float x, float y)
                : x(x), y(y)
            {
            }

            float x;
            float y;

            static Vector2 Zero()
            {
                return Vector2(0, 0);
            }
    };

    enum class VMState {
        CLIENT,
        SERVER
    };
}

#endif /* !TYPES_HPP_ */
