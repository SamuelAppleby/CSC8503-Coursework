#pragma once
#include "CubeObject.h"
namespace NCL {
	namespace CSC8503 {
		class SpringCubeObject : public CubeObject {
		public:
			SpringCubeObject() {
				invMass = 2.0f;
				name = "SpringCube";
				restPosition = Vector3();
			}
			Vector3 GetRestPosition() const {
				return restPosition;
			}
			void SetRestPosition(Vector3 val) {
				restPosition = val;
			}
		protected:
			Vector3 restPosition;
		};
	}
}