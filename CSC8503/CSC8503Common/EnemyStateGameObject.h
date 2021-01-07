#pragma once
#include "StateGameObject.h"
#include "Debug.h"
#include "NavigationGrid.h"
namespace NCL {
	namespace CSC8503 {
		class EnemyStateGameObject : public StateGameObject {
		public:
			EnemyStateGameObject(GameObject* follow);
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
			void Idle();
			void FollowPlayer(float dt);
		protected:
			State* followPlayerState;
			GameObject* object;
			Vector3 travelDir;
			float rayTime;
			bool seePlayer;
			bool finished;
		};
	}
}
