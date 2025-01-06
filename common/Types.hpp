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

            Vector2 operator+(const Vector2 &other) const
            {
                return Vector2(x + other.x, y + other.y);
            }

            Vector2 operator+=(const Vector2 &other)
            {
                x += other.x;
                y += other.y;
                return *this;
            }

            Vector2 operator+(const float &other) const
            {
                return Vector2(x + other, y + other);
            }

            Vector2 operator+=(const float &other)
            {
                x += other;
                y += other;
                return *this;
            }

            Vector2 operator*(const Vector2 &other) const
            {
                return Vector2(x * other.x, y * other.y);
            }

            Vector2 operator*=(const Vector2 &other)
            {
                x *= other.x;
                y *= other.y;
                return *this;
            }

            Vector2 operator*(const float &other) const
            {
                return Vector2(x * other, y * other);
            }

            Vector2 operator*=(const float &other)
            {
                x *= other;
                y *= other;
                return *this;
            }
    };

    class Rect2 {
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
    };

    enum class VMState {
        CLIENT,
        SERVER
    };
}

#endif /* !TYPES_HPP_ */
