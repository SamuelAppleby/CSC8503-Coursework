#pragma once
#include "CubeObject.h"
namespace NCL {
	namespace CSC8503 {
		class ProjectileCubeObject : public CubeObject {
		public:
			ProjectileCubeObject() {
				invMass = 2.0f;
				name = "ProjectileCube";
			}
		};
	}
}