#pragma once
#include "FloorObject.h"

namespace NCL {
	namespace CSC8503 {
		class OrientedFloorObject : public FloorObject {
		public:
			OrientedFloorObject(Matrix4 rotation) {
				name = "OrientedFloor";
				this->GetTransform().SetOrientation(rotation);
			}
		};
	}
}