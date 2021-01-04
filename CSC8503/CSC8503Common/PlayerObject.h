#pragma once
#include "GameObject.h"
namespace NCL {
	namespace CSC8503 {
		class PlayerObject : public GameObject {
		public:
			PlayerObject() {
				name = "Player";
				invMass = 5.0f;
				elasticity = 0.2;
				friction = 0.5;
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
				canJump = true;
			}
			void OnCollisionEnd(GameObject* otherObject) override {
				//canJump = false;
			}
			void Jump() {
				if (canJump)
					physicsObject->ApplyLinearImpulse(Vector3(0, 5, 0));
			}
			int GetScore() const {
				return score;
			}
			void BonusAcquired() {
				score += 25;
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