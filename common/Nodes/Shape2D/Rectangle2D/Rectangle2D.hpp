//
// Created by Yanis Moyon on 06/01/2025.
//

#ifndef RECTANGLE2D_HPP
#define RECTANGLE2D_HPP

  #include "Nodes/Shape2D/Shape2D.hpp"

class Rectangle2D : public Shape2D {
private:
  Types::Rect2 dimensions;

  bool _intersectsRect(const Shape2D& other) const override;

  bool _intersectsCircle(const Shape2D& circle) const override;

public:
  Rectangle2D(const Types::Vector2 &origin, const Types::Vector2 &size);

  void setDimensions(float w, float h);

  Types::Rect2 getDimensions() const;

  float getWidth() const;

  float getHeight() const;

  Types::Vector2 getOrigin() const override;

  bool intersects(const Shape2D& other) const override;

  Types::Rect2 getBoundingBox() const override;

};


#endif //RECTANGLE2D_HPP
