#pragma once
#include "FloorObject.h"
#include "PlayerObject.h"
#include "PathFindingStateGameObject.h"
#include "PatrolStateGameObject.h"

namespace NCL {
	namespace CSC8503 {
		class LavaObject : public FloorObject {
		public:
			LavaObject() {
				name = "Lava";
			}
			void OnCollisionBegin(GameObject* otherObject) override {
				otherObject->GetPhysicsObject()->ClearForces();
				otherObject->GetPhysicsObject()->SetLinearVelocity(Vector3(0, 0, 0));
				otherObject->GetPhysicsObject()->SetAngularVelocity(Vector3(0, 0, 0));
				if (PlayerObject* o = dynamic_cast<PlayerObject*>(otherObject)) {
					o->GetTransform().SetPosition(o->GetSpawnPos());
				}
				else if (PathFindingStateGameObject* o = dynamic_cast<PathFindingStateGameObject*>(otherObject)) {
					o->GetTransform().SetPosition(o->GetPath()[0]);
				}
				else if (PatrolStateGameObject* o = dynamic_cast<PatrolStateGameObject*>(otherObject)) {
					o->GetTransform().SetPosition(o->GetRoute()[o->GetCurrentDest()]);
				}
				else if (EnemyStateGameObject* o = dynamic_cast<EnemyStateGameObject*>(otherObject)) {
					o->GetTransform().SetPosition(o->GetFollowObject()->GetTransform().GetPosition() + Vector3(0, 5, 10));
				}
				else if (!dynamic_cast<StateGameObject*>(otherObject)) {		// Don't ever delete state objects
					otherObject->SetIsActive(false);
				}
			}
		};
	}
}