/*
** EPITECH PROJECT, 2025
** RTYPE
** File description:
** Area2D
*/

#include "Nodes/Node2D/CollisionNode2D/CollisionNode2D.hpp"
#include "raylib.h"

class Area2D : public CollisionNode2D {
    public:
        explicit Area2D(const std::string &name = "area2d");
        ~Area2D();

        void addCollisionShape(CollisionShape2D* new_shape) = delete;
        void removeCollisionShape(const std::string &name) = delete;

        Types::Vector2 getSize() const;
        void setSize(const Types::Vector2 &size);

    private:
        Types::Vector2 _size;

};
