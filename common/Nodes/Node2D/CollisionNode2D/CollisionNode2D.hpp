//
// Created by Yanis Moyon on 07/01/2025.
//

#ifndef COLLISIONNODE2D_HPP
#define COLLISIONNODE2D_HPP

#include <memory>

#include <Nodes/Node2D/Node2D.hpp>
#include <Nodes/Node2D/CollisionShape2D/CollisionShape2D.hpp>

class CollisionNode2D: public Node2D {
	std::vector<std::unique_ptr<CollisionShape2D>> shapes;
    bool collision_enabled;

public:
	explicit CollisionNode2D(const std::string &name = "collisionnode2d");

    void addCollisionShape(CollisionShape2D* new_shape);

    void removeCollisionShape(const std::string &name);

    CollisionShape2D* getCollisionShape(const std::string &name) const;

    std::vector<CollisionShape2D*> getCollisionShapes() const;

	Types::Rect2 getBoundingBox() const;

    void toggleCollision();

    bool isCollisionEnabled() const;

    bool collidesWith(const CollisionNode2D& other);
};

#endif //COLLISIONNODE2D_HPP
