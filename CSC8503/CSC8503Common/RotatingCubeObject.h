#pragma once
#include "CubeObject.h"
namespace NCL {
	namespace CSC8503 {
		class RotatingCubeObject : public CubeObject {
		public:
			RotatingCubeObject() {
				invMass = 0.0f;
				name = "RotatingCube";
			}
		};
	}
}