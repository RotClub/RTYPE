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
            Vector2() : x(0), y(0) {}

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

            Vector2 operator+(const Vector2 &other)
            {
                return Vector2(x + other.x, y + other.y);
            }

            Vector2 operator+=(const Vector2 &other)
            {
                x += other.x;
                y += other.y;
                return *this;
            }

            Vector2 operator+(const float &other)
            {
                return Vector2(x + other, y + other);
            }

            Vector2 operator+=(const float &other)
            {
                x += other;
                y += other;
                return *this;
            }
    };

    enum class VMState {
        CLIENT,
        SERVER
    };
}

#endif /* !TYPES_HPP_ */
