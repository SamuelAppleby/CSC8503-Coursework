#pragma once
#include "GameObject.h"
#include "LavaObject.h"
#include "BonusObject.h"
namespace NCL {
	namespace CSC8503 {
		class PlayerObject : public GameObject {
		public:
			PlayerObject() {
				name = "Player";
				invMass = 0.5f;
				elasticity = 0.2;
				friction = 0.8;
				canJump = false;
				score = 1000;
			}
			void SetPhysicsObject(PhysicsObject* newObject) override {
				physicsObject = newObject;
				physicsObject->SetInverseMass(invMass);
				physicsObject->SetElasticity(elasticity);
				physicsObject->SetFriction(friction);
			}
			void OnCollisionBegin(GameObject* otherObject) override {
				if (dynamic_cast<LavaObject*>(otherObject)) {
					physicsObject->ClearForces();
					physicsObject->SetLinearVelocity(Vector3(0, 0, 0));
					physicsObject->SetAngularVelocity(Vector3(0, 0, 0));
					transform.SetPosition(Vector3(0, 10, 0));
				}
				if (dynamic_cast<BonusObject*>(otherObject)) {
					score += 25;
					otherObject->SetIsActive(false);
				}
				canJump = true;
			}
			void OnCollisionEnd(GameObject* otherObject) override {
				canJump = false;
			}
			void Jump() {
				if (canJump)
					physicsObject->ApplyLinearImpulse(Vector3(0, 50, 0));
			}
			int GetScore() const {
				return score;
			}
			void DecreaseScore(float dt) {
				score -= (10 * dt);
			}
		protected:
			float invMass;
			float elasticity;
			float friction;

			bool canJump;
			float score;
		};
	}
}