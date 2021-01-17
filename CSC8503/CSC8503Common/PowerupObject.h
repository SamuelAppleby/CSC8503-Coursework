#pragma once
#include "PlayerObject.h"
#include "PickupObject.h"
namespace NCL {
	namespace CSC8503 {
		class PowerupObject : public PickupObject {
		public:
			PowerupObject() {
				name = "Bonus";
			}
			void OnCollisionBegin(GameObject* otherObject) override {
				if (dynamic_cast<PlayerObject*>(otherObject) || dynamic_cast<EnemyStateGameObject*>(otherObject)) {
					otherObject->SetPowerUpTimer(5.0f);
					isActive = false;
				}
			}
		};
	}
}