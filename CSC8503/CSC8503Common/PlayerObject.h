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
				friction = 0.3;
				canJump = false;
				score = 1000;
				finished = false;
			}
			void SetPhysicsObject(PhysicsObject* newObject) override {
				physicsObject = newObject;
				physicsObject->SetInverseMass(invMass);
				physicsObject->SetElasticity(elasticity);
				physicsObject->SetFriction(friction);
			}
			void OnCollisionBegin(GameObject* otherObject) override {
				if(!canJump)
					canJump = true;
			}
			void OnCollisionEnd(GameObject* otherObject) override {
				if(canJump)
					canJump = false;
			}
			void Jump() {
				//if (canJump)
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
			bool GetFinished() const {
				return finished;
			}
			void SetFinished(bool val) {
				finished = val;
			}
			Vector3 GetSpawnPos() const {
				return spawnPos;
			}
			void SetSpawnPos(Vector3 val) {
				spawnPos = val;
			}
			void Update(float dt) {
				if (powerUpTimer > 0.0f) {
					GetRenderObject()->SetColour(Vector4(0, 1, 10, 1));
					powerUpTimer -= dt;
				}
				else {
					powerUpTimer -= dt;
					GetRenderObject()->SetColour(Vector4(0, 0.5, 1, 1));
				}
			}
		protected:
			float invMass;
			float elasticity;
			float friction;
			bool canJump;
			float score;
			bool finished;
			Vector3 spawnPos;
		};
	}
}