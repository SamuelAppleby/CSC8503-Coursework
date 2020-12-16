#pragma once
#include "FloorObject.h"
#include "SphereObject.h"
#include "CubeObject.h"

namespace NCL {
	namespace CSC8503 {
		class LavaObject : public FloorObject {
		public:
			LavaObject() {
				name = "Lava";
			}
			void OnCollisionBegin(GameObject* otherObject) override {
				if (dynamic_cast<SphereObject*>(otherObject) || dynamic_cast<CubeObject*>(otherObject))
					otherObject->SetIsActive(false);
			}
		};
	}
}