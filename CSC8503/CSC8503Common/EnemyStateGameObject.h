#pragma once
#include "StateGameObject.h"
#include "Debug.h"
#include "NavigationGrid.h"
namespace NCL {
	namespace CSC8503 {
		class EnemyStateGameObject : public StateGameObject {
		public:
			EnemyStateGameObject(GameObject* o);
			float GetRayTime() const {
				return rayTime;
			}
			void SetRayTime(float val) {
				rayTime = val;
			}
			bool GetFinished() const {
				return finished;
			}
			void SetFinished(bool val) {
				finished = val;
			}
			void SetFollowTimeOut(float val) {
				followTimeout = val;
			}
			GameObject* GetFollowObject() const {
				return object;
			}
			void Update(float dt) override;
			void FollowPlayer(float dt);
		protected:
			State* followPlayerState;
			GameObject* object;
			Vector3 travelDir;
			float rayTime;
			bool finished;
			float followTimeout;
		};
	}
}
