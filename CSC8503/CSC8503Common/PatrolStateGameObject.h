#pragma once
#include "EnemyStateGameObject.h"
#include "Debug.h"
#include "NavigationGrid.h"
namespace NCL {
	namespace CSC8503 {
		class PatrolStateGameObject : public EnemyStateGameObject {
		public:
			PatrolStateGameObject(vector<Vector3> positions);
			vector<Vector3> GetRoute() const {
				return route;
			}
			int GetCurrentDest() const {
				return currentDest;
			}
		protected:
			void Patrol(float dt);
			void DisplayRoute();
			vector<Vector3> route;
			State* patrolState;
			int currentDest;
			float routeTimeout;
		};
	}
}
