#pragma once
#include "GameObject.h"
#include "PlayerObject.h"
#include "EnemyObject.h"
#include "PatrolStateGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class BonusObject : public GameObject {
		public:
			BonusObject() {
				name = "Bonus";
				invMass = 0.0f;
				elasticity = 0.0;
				friction = 0.8;
			}
			void SetPhysicsObject(PhysicsObject* newObject) override {
				physicsObject = newObject;
				physicsObject->SetInverseMass(invMass);
				physicsObject->SetElasticity(elasticity);
				physicsObject->SetFriction(friction);
			}
			void OnCollisionBegin(GameObject* otherObject) override {
				if (dynamic_cast<PlayerObject*>(otherObject) || dynamic_cast<EnemyObject*>(otherObject)
					|| dynamic_cast<PatrolStateGameObject*>(otherObject)) {
					if (PlayerObject* p = dynamic_cast<PlayerObject*>(otherObject))
						p->BonusAcquired();
					isActive = false;
				}
			}
		protected:
			float invMass;
			float elasticity;
			float friction;
		};
	}
}