#pragma once
#include "FloorObject.h"

namespace NCL {
	namespace CSC8503 {
		class IceObject : public FloorObject {
		public:
			IceObject() {
				friction = 0.1;
				name = "Ice";
			}
		};
	}
}