#pragma once
#include "GameObject.h"
namespace NCL {
	namespace CSC8503 {
		class SpringObject : public GameObject {
		public:
			SpringObject(Vector3 rest) {
				invMass = 20.0f;
				elasticity = 0.2;
				friction = 0.8;
				name = "SpringCube";
				restPosition = rest;
			}
			void SetPhysicsObject(PhysicsObject* newObject) override {
				physicsObject = newObject;
				physicsObject->SetInverseMass(invMass);
				physicsObject->SetElasticity(elasticity);
				physicsObject->SetFriction(friction);
			}
			Vector3 GetRestPosition() const {
				return restPosition;
			}
			void SetRestPosition(Vector3 val) {
				restPosition = val;
			}
		protected:
			float invMass;
			float elasticity;
			float friction;
			Vector3 restPosition;
		};
	}
}