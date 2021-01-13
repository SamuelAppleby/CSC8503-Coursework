#pragma once
#include "EnemyStateGameObject.h"
#include "Debug.h"
#include "NavigationGrid.h"
namespace NCL {
	namespace CSC8503 {
		class PathFindingStateGameObject : public EnemyStateGameObject {
		public:
			PathFindingStateGameObject(GameObject* follow, bool ignoreCosts);
			vector<Vector3> GetPath() const {
				return path;
			}
		protected:
			void FindPath();
			void DisplayPath();
			void FollowPath(float dt);
			vector<Vector3> path;
			Vector3 mazeStart;
			Vector3 mazeEnd;
			State* followPathState;
			bool ignoreCosts;
			float pathTimeout;
		};
	}
}
