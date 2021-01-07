#pragma once
#include "StateGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class PlatformStateGameObject : public StateGameObject {
		public:
			PlatformStateGameObject(Vector3 start, Vector3 end);
			void SetPhysicsObject(PhysicsObject* newObject) override {
				physicsObject = newObject;
				physicsObject->SetInverseMass(0.0f);
				physicsObject->SetElasticity(0.2f);
				physicsObject->SetFriction(0.8f);
			}
		protected:
			void Idle(float dt);
			void MoveToLocation(float dt);
			float xSpeed;
			float ySpeed;
			float zSpeed;
			Vector3 start;
			Vector3 end;
			bool moving;
			float invMass;
			float elasticity;
			float friction;
		};
	}
}
