/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Types
*/

#ifndef TYPES_HPP_
#define TYPES_HPP_

namespace Types
{
    class Vector2
    {
        public:
            Vector2() : x(0), y(0) {}

            Vector2(float x, float y) : x(x), y(y) {}

            float x;
            float y;

            static Vector2 Zero() { return Vector2(0, 0); }

            Vector2 operator+(const Vector2 &other) const { return Vector2(x + other.x, y + other.y); }

            Vector2 operator+=(const Vector2 &other)
            {
                x += other.x;
                y += other.y;
                return *this;
            }

            Vector2 operator+(const float &other) const { return Vector2(x + other, y + other); }

            Vector2 operator+=(const float &other)
            {
                x += other;
                y += other;
                return *this;
            }

            Vector2 operator*(const Vector2 &other) const { return Vector2(x * other.x, y * other.y); }

            Vector2 operator*=(const Vector2 &other)
            {
                x *= other.x;
                y *= other.y;
                return *this;
            }

            Vector2 operator*(const float &other) const { return Vector2(x * other, y * other); }

            Vector2 operator*=(const float &other)
            {
                x *= other;
                y *= other;
                return *this;
            }
    };

    class Rect2
    {
        public:
            Vector2 origin;
            Vector2 size;

            bool intersects(const Rect2 &other) const
            {
                float this_left = origin.x;
                float this_right = this_left + size.x;
                float this_top = origin.y;
                float this_bottom = this_right + size.y;

                float other_left = other.origin.x;
                float other_right = other_left + other.size.x;
                float other_top = other.origin.y;
                float other_bottom = other_right + other.size.y;

                bool horizontal_overlap = this_left < other_right && this_right > other_left;
                bool vertical_overlap = this_top < other_bottom && this_bottom > other_top;

                return horizontal_overlap && vertical_overlap;
            }

            void expands(const Rect2 &to_expand)
            {
                float new_left = origin.x < to_expand.origin.x ? origin.x : to_expand.origin.x;
                float new_top = origin.y < to_expand.origin.y ? origin.y : to_expand.origin.y;
                float new_right = origin.x + size.x > to_expand.origin.x + to_expand.size.x
                    ? origin.x + size.x
                    : to_expand.origin.x + to_expand.size.x;
                float new_bottom = origin.y + size.y > to_expand.origin.y + to_expand.size.y
                    ? origin.y + size.y
                    : to_expand.origin.y + to_expand.size.y;

                origin.x = new_left;
                origin.y = new_top;
                size.x = new_right - new_left;
                size.y = new_bottom - new_top;
            }
    };

    enum class VMState
    {
        CLIENT,
        SERVER
    };
} // namespace Types

#endif /* !TYPES_HPP_ */
