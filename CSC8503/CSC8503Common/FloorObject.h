#pragma once
#include "GameObject.h"
namespace NCL {
	namespace CSC8503 {
		class FloorObject : public GameObject {
		public:
			FloorObject() {
				invMass = 0.0f;
				elasticity = 0.0f;
				friction = 0.5f;
				name = "Floor";
			}
			void SetPhysicsObject(PhysicsObject* newObject) override {
				physicsObject = newObject;
				physicsObject->SetInverseMass(invMass);
				physicsObject->SetElasticity(elasticity);
				physicsObject->SetFriction(friction);
			}
		protected:
			float invMass;
			float elasticity;
			float friction;
		};

	}
}