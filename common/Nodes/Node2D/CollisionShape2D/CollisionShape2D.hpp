//
// Created by Yanis Moyon on 18/12/2024.
//

#ifndef COLLISIONSHAPE2D_HPP
#define COLLISIONSHAPE2D_HPP

#include "Nodes/Node2D/Node2D.hpp"
#include "Nodes/Shape2D/Shape2D.hpp"

class CollisionShape2D : public Node2D {
private:
    Shape2D* shape;
    bool collision_enabled;

public:
    CollisionShape2D();

    void setShape(Shape2D* new_shape);

    Shape2D* getShape() const;

	Types::Rect2 getBoundingBox() const;

    void toggleCollision();

    bool isCollisionEnabled() const;

    bool collidesWith(const CollisionShape2D& other);
};


#endif //COLLISIONSHAPE2D_HPP
