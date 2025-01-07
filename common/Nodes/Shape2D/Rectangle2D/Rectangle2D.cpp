//
// Created by Yanis Moyon on 06/01/2025.
//

#include "Rectangle2D.hpp"

bool Rectangle2D::_intersectsRect(const Shape2D& other) const {
  Types::Rect2 other_bb = other.getBoundingBox();
  return dimensions.intersects(other_bb);
}

Rectangle2D::Rectangle2D(const Types::Vector2 &origin, const Types::Vector2 &size)
	: dimensions(Types::Rect2(origin, size)) {

}

void Rectangle2D::setDimensions(float w, float h) {
  dimensions.size.x = w;
  dimensions.size.y = h;
}

Types::Rect2 Rectangle2D::getDimensions() const {
  return dimensions;
}

float Rectangle2D::getWidth() const {
  return dimensions.size.x;
}

float Rectangle2D::getHeight() const {
  return dimensions.size.y;
}

Types::Vector2 Rectangle2D::getOrigin() const {
  return dimensions.origin;
}

bool Rectangle2D::intersects(const Shape2D& other) const {
	std::cout << "Last condition" << std::endl;
	if (other.shape == Shape2D::ShapeType::RECTANGLE) {
		return _intersectsRect(other);
	}
}

Types::Rect2 Rectangle2D::getBoundingBox() const {
  return dimensions;
}
