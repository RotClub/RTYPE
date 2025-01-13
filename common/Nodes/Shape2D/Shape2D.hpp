//
// Created by Yanis Moyon on 06/01/2025.
//

#ifndef SHAPE2D_HPP
#define SHAPE2D_HPP

#include "Types.hpp"

class Shape2D {
public:
	enum class ShapeType {
		RECTANGLE,
		CIRCLE
	} shape;

    virtual Types::Vector2 getOrigin() const = 0;

	virtual void setOrigin(float x, float y) = 0;

	bool intersects(const Shape2D& other) const;

	virtual Types::Rect2 getBoundingBox() const = 0;

private:
	virtual bool _intersectsRect(const Shape2D& other) const = 0;

	virtual bool _intersectsCircle(const Shape2D& other) const = 0;
};

#endif //SHAPE2D_HPP
