#pragma once
#include "StateGameObject.h"
#include "Debug.h"
#include <set>
namespace NCL {
	namespace CSC8503 {
		class EnemyStateGameObject : public StateGameObject {
		public:
			EnemyStateGameObject();
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
			void AddFollowObject(GameObject* o) {
				interestObjects.insert(o);
			}
			void ClearFollowObjects() {
				interestObjects.clear();
			}
			Vector3 GetTravelDirection() const {
				return travelDir;
			}
			GameObject* GetCurrentObject() const {
				return currentObject;
			}
			void SetDisplayDirection(float val) {
				displayPath = val;
			}
			void Update(float dt) override;
			void FollowObject(float dt);
			void DisplayDirection();
		protected:
			State* followObjectState;
			std::set<GameObject*> interestObjects;
			GameObject* currentObject;
			Vector3 travelDir;
			float rayTime;
			bool finished;
			float followTimeout;
			float speed;
			bool displayPath;
		};
	}
}
