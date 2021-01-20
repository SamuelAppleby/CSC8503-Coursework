#pragma once
#include "FloorObject.h"
#include "PlayerObject.h"
#include "PathFindingEnemyStateGameObject.h"
#include "PatrolEnemyStateGameObject.h"

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
				if (PlayerObject* o = dynamic_cast<PlayerObject*>(otherObject)) 
					o->GetTransform().SetPosition(o->GetSpawnPos());
				else if (EnemyStateGameObject* o = dynamic_cast<EnemyStateGameObject*>(otherObject)) {
					o->ClearFollowObjects();
					if (PathFindingEnemyStateGameObject* p = dynamic_cast<PathFindingEnemyStateGameObject*>(otherObject))
						p->GetTransform().SetPosition(p->GetPath()[0] + Vector3(0, 10, 0));
					if (PatrolEnemyStateGameObject* s = dynamic_cast<PatrolEnemyStateGameObject*>(otherObject))
						s->GetTransform().SetPosition(s->GetRoute()[s->GetCurrentDest()] + Vector3(0, 10, 0));
				}
				else if (!dynamic_cast<StateGameObject*>(otherObject))		// Don't ever delete state objects
					otherObject->SetIsActive(false);
			}
		};
	}
}