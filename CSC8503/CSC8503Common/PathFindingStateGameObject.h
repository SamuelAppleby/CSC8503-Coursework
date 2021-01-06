#pragma once
#include "StateGameObject.h"
#include "Debug.h"
#include "NavigationGrid.h"
namespace NCL {
	namespace CSC8503 {
		class PathFindingStateGameObject : public StateGameObject {
		public:
			PathFindingStateGameObject(Vector3 pos, GameObject* follow, NavigationGrid* map);
			float GetRayTime() const {
				return rayTime;
			}
			void SetRayTime(float val) {
				rayTime = val;
			}
			bool GetSeePlayer() const {
				return seePlayer;
			}
			void SetSeePlayer(bool val) {
				seePlayer = val;
			}
			bool GetFinished() const {
				return finished;
			}
			void SetFinished(bool val) {
				finished = val;
			}
			void Update(float dt) override;
		protected:
			void FindPath();
			void DisplayPath();
			void FollowPath(float dt);
			void FollowPlayer(float dt);
			GameObject* object;
			Vector3 playerDirection;
			Vector3 pathdir;
			vector<Vector3> path;
			Vector3 mazeStart;
			Vector3 mazeEnd;
			NavigationGrid* grid;
			float rayTime;
			bool seePlayer;
			bool finished;
		};
	}
}
