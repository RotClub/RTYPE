/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Types
*/

#ifndef TYPES_HPP_
#define TYPES_HPP_

#include <algorithm>
#ifdef WIN32
#include <WindowsCross.hpp>
#endif

namespace Types
{
    class Vector3
    {
        public:
            Vector3() : x(0), y(0), z(0) {}

            Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

            float x;
            float y;
            float z;

            static Vector3 Zero() { return Vector3(0, 0, 0); }

            Vector3 operator+(const Vector3 &other) const { return Vector3(x + other.x, y + other.y, z + other.z); }

            Vector3 operator+=(const Vector3 &other)
            {
                x += other.x;
                y += other.y;
                z += other.z;
                return *this;
            }

            Vector3 operator+(const float &other) const { return Vector3(x + other, y + other, z + other); }

            Vector3 operator+=(const float &other)
            {
                x += other;
                y += other;
                z += other;
                return *this;
            }

            Vector3 operator*(const Vector3 &other) const { return Vector3(x * other.x, y * other.y, z * other.z); }

            Vector3 operator*=(const Vector3 &other)
            {
                x *= other.x;
                y *= other.y;
                z *= other.z;
                return *this;
            }

            Vector3 operator*(const float &other) const { return Vector3(x * other, y * other, z * other); }

            Vector3 operator*=(const float &other)
            {
                x *= other;
                y *= other;
                z *= other;
                return *this;
            }
    };

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
                float left = origin.x;
                float right = origin.x + size.x;
                float top = origin.y;
                float bottom = origin.y + size.y;

                float other_left = other.origin.x;
                float other_right = other.origin.x + other.size.x;
                float other_top = other.origin.y;
                float other_bottom = other.origin.y + other.size.y;

#ifdef WIN32
                float x_overlap = max(0.f, min(right, other_right) - max(left, other_left));
                float y_overlap = max(0.f, min(bottom, other_bottom) - max(top, other_top));
#else
                float x_overlap = std::max(0.f, std::min(right, other_right) - std::max(left, other_left));
                float y_overlap = std::max(0.f, std::min(bottom, other_bottom) - std::max(top, other_top));
#endif
                return x_overlap > 0 && y_overlap > 0;
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
