#pragma once
#include "CubeObject.h"
namespace NCL {
	namespace CSC8503 {
		class ProjectileCubeObject : public CubeObject {
		public:
			ProjectileCubeObject() {
				invMass = 0.5f;
				name = "ProjectileCube";
			}
		};
	}
}