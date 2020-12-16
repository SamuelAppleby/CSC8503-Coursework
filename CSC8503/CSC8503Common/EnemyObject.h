#pragma once
#include "GameObject.h"
#include "LavaObject.h"
#include "BonusObject.h"
namespace NCL {
	namespace CSC8503 {
		class EnemyObject : public GameObject {
		public:
			EnemyObject() {
				name = "Enemy";
				invMass = 0.5f;
				elasticity = 0.2;
				friction = 0.8;
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