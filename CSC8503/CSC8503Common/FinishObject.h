#pragma once
#include "FloorObject.h"
#include "PathFindingStateGameObject.h"
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
				else if (PathFindingStateGameObject* p = dynamic_cast<PathFindingStateGameObject*>(otherObject)) {
					p->SetFinished(true);
				}
			}
		};
	}
}