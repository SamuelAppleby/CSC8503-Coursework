#pragma once
#include "FloorObject.h"

namespace NCL {
	namespace CSC8503 {
		class TrampolineObject : public FloorObject {
		public:
			TrampolineObject() {
				elasticity = 10.0f;
				name = "Trampoline";
			}
		};
	}
}