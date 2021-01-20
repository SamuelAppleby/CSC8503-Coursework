#pragma once
#include "FloorObject.h"
#include "PathFindingEnemyStateGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class FinishObject : public FloorObject {
		public:
			FinishObject() {
				name = "Finish";
			}
			void OnCollisionBegin(GameObject* otherObject) override {
				if (PlayerObject* p = dynamic_cast<PlayerObject*>(otherObject)) {
					p->SetFinished(true);
				}
				else if (PathFindingEnemyStateGameObject* p = dynamic_cast<PathFindingEnemyStateGameObject*>(otherObject)) {
					p->SetFinished(true);
				}
			}
		};
	}
}