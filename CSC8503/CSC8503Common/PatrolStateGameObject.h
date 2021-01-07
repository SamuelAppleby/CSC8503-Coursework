#pragma once
#include "EnemyStateGameObject.h"
#include "Debug.h"
#include "NavigationGrid.h"
namespace NCL {
	namespace CSC8503 {
		class PatrolStateGameObject : public EnemyStateGameObject {
		public:
			PatrolStateGameObject(GameObject* follow, vector<Vector3> positions);
		protected:
			void Patrol(float dt);
			vector<Vector3> locations;
			State* patrolState;
		};
	}
}
