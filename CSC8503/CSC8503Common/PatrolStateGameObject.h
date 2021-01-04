#pragma once
#include "StateGameObject.h"
namespace NCL {
	namespace CSC8503 {
		class PatrolStateGameObject : public StateGameObject {
		public:
			PatrolStateGameObject(GameObject* follow);
			void Update(float dt) override;
		protected:
			void Patrol(float dt);
			void Follow(float dt);
			GameObject* object;
			Vector3 direction;
		};
	}
}
