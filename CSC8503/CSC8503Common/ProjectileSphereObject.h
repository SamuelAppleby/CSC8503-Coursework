#pragma once
#include "SphereObject.h"
namespace NCL {
	namespace CSC8503 {
		class ProjectileSphereObject : public SphereObject {
		public:
			ProjectileSphereObject() {
				invMass = 0.5f;
				name = "ProjectileSphere";
			}
		};
	}
}