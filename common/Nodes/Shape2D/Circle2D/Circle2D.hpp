/*
** EPITECH PROJECT, 2025
** RTYPE
** File description:
** Circle2D collision system
*/

#ifndef CIRCLE2D_HPP
#define CIRCLE2D_HPP

    #include "Nodes/Shape2D/Shape2D.hpp"

class Circle2D : public Shape2D {
        float _radius;
        Types::Vector2 _origin;

        bool _intersectsCircle(const Shape2D& other) const override;

        bool _intersectsRect(const Shape2D &rect) const override;

    public:
        Circle2D(const Types::Vector2 &origin, float radius);

        void setRadius(float radius);

        float getRadius() const;

		void setOrigin(const Types::Vector2 &origin);

        Types::Vector2 getOrigin() const override;

        Types::Rect2 getBoundingBox() const override;
};

#endif //CIRCLE2D_HPP
