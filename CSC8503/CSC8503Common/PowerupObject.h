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
				if (EnemyStateGameObject* o = dynamic_cast<EnemyStateGameObject*>(otherObject)) {
					o->SetPowerUpTimer(5.0f);
					o->GetRenderObject()->SetColour(Vector4(10, 1, 0, 1));
				}
				if (PlayerObject* o = dynamic_cast<PlayerObject*>(otherObject)) {
					o->SetPowerUpTimer(5.0f);
					o->GetRenderObject()->SetColour(Vector4(0, 1, 10, 1));
				}
				isActive = false;
			}
		};
	}
}