#pragma once
#include "GameObject.h"
#include "PlayerObject.h"
#include "EnemyStateGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class PickupObject : public GameObject {
		public:
			PickupObject() {
				name = "Pickup";
				invMass = 0.0f;
				elasticity = 0.0;
				friction = 0.8;
				canCollide = false;
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

