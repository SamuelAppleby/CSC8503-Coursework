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
				canCollide = false;
			}
			void SetPhysicsObject(PhysicsObject* newObject) override {
				physicsObject = newObject;
				physicsObject->SetInverseMass(invMass);
				physicsObject->SetElasticity(elasticity);
				physicsObject->SetFriction(friction);
			}
			void OnCollisionBegin(GameObject* otherObject) override {
				if (dynamic_cast<PlayerObject*>(otherObject) || dynamic_cast<EnemyStateGameObject*>(otherObject)) {
					if (PlayerObject* p = dynamic_cast<PlayerObject*>(otherObject))
						p->BonusAcquired();
					if (EnemyStateGameObject* e = dynamic_cast<EnemyStateGameObject*>(otherObject)) {
						e->RemoveFollowObject(this);
						e->SetFollowTimeOut(0.0f);
					}
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