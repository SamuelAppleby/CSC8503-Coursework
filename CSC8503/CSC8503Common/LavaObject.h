#pragma once
#include "FloorObject.h"
#include "PlayerObject.h"
namespace NCL {
	namespace CSC8503 {
		class LavaObject : public FloorObject {
		public:
			LavaObject() {
				name = "Lava";
			}
			void OnCollisionBegin(GameObject* otherObject) override {
				if (PlayerObject* p = dynamic_cast<PlayerObject*>(otherObject)) {
					p->GetPhysicsObject()->ClearForces();
					p->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));
					p->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 0, 0));
					p->GetTransform().SetPosition(Vector3(0, 10, 0));
				}
				else 
					otherObject->SetIsActive(false);
			}
		};
	}
}