//
// Created by Yanis Moyon on 13/01/2025.
//

#ifndef BODY2D_HPP
#define BODY2D_HPP

#include "Nodes/Node2D/CollisionNode2D/CollisionNode2D.hpp"

class Body2D : public CollisionNode2D {
		Types::Vector2 velocity;
	public:
		void Update() override;

};

#endif //BODY2D_HPP
