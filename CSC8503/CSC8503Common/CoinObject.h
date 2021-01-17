#pragma once
#include "PickupObject.h"
namespace NCL {
	namespace CSC8503 {
		class CoinObject : public PickupObject {
		public:
			CoinObject() {
				name = "Bonus";
			}
			void OnCollisionBegin(GameObject* otherObject) override {
				if (dynamic_cast<PlayerObject*>(otherObject) || dynamic_cast<EnemyStateGameObject*>(otherObject))
					isActive = false;
				if (PlayerObject* p = dynamic_cast<PlayerObject*>(otherObject))
					p->BonusAcquired();
			}
		};
	}
}