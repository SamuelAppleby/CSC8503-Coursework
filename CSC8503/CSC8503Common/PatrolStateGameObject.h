#pragma once
#include "StateGameObject.h"
#include "Debug.h"
#include "NavigationGrid.h"
namespace NCL {
	namespace CSC8503 {
		class PatrolStateGameObject : public StateGameObject {
		public:
			PatrolStateGameObject(vector<Vector3> positions, GameObject* follow);
			void Update(float dt) override;
		protected:
			void Patrol(float dt);
			void FollowPlayer(float dt);
			GameObject* object;
			Vector3 direction;
			vector<Vector3> locations;
		};
	}
}
