#pragma once
#include "StateGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class OscillateStateGameObject : public StateGameObject {
		public:
			OscillateStateGameObject();
		protected:
			void MoveLeft(float dt);
			void MoveRight(float dt);
			float counter;
		};
	}
}
