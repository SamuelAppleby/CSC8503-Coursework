#pragma once
#include "CubeObject.h"
namespace NCL {
	namespace CSC8503 {
		class PlatformCubeObject : public CubeObject {
		public:
			PlatformCubeObject() {
				invMass = 0.0f;
				name = "PlatformCube";
			}
		};
	}
}